#include "deepcoin/ws_public.h"

#include <arpa/inet.h>
#include <netdb.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <openssl/ssl.h>
#include <sys/socket.h>
#include <unistd.h>

#include <atomic>
#include <chrono>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

namespace deepcoin {
namespace {

class Fd {
 public:
  explicit Fd(int fd = -1) : fd_(fd) {}
  ~Fd() {
    if (fd_ >= 0) {
      close(fd_);
    }
  }
  Fd(const Fd&) = delete;
  Fd& operator=(const Fd&) = delete;
  Fd(Fd&& other) noexcept : fd_(other.release()) {}
  Fd& operator=(Fd&& other) noexcept {
    if (this != &other) {
      if (fd_ >= 0) {
        close(fd_);
      }
      fd_ = other.release();
    }
    return *this;
  }
  int get() const { return fd_; }
  int release() {
    int fd = fd_;
    fd_ = -1;
    return fd;
  }

 private:
  int fd_;
};

std::string base64(const unsigned char* data, size_t size) {
  BIO* b64 = BIO_new(BIO_f_base64());
  BIO* mem = BIO_new(BIO_s_mem());
  BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
  BIO_push(b64, mem);
  BIO_write(b64, data, static_cast<int>(size));
  BIO_flush(b64);
  BUF_MEM* buffer = nullptr;
  BIO_get_mem_ptr(b64, &buffer);
  std::string result(buffer->data, buffer->length);
  BIO_free_all(b64);
  return result;
}

int connect_tcp(const std::string& host, const std::string& port) {
  addrinfo hints{};
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  addrinfo* addresses = nullptr;
  const int rc = getaddrinfo(host.c_str(), port.c_str(), &hints, &addresses);
  if (rc != 0) {
    throw std::runtime_error(gai_strerror(rc));
  }

  Fd fd;
  for (addrinfo* addr = addresses; addr != nullptr; addr = addr->ai_next) {
    fd = Fd(socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol));
    if (fd.get() < 0) {
      continue;
    }
    if (connect(fd.get(), addr->ai_addr, addr->ai_addrlen) == 0) {
      freeaddrinfo(addresses);
      return fd.release();
    }
  }

  freeaddrinfo(addresses);
  throw std::runtime_error("failed to connect to " + host + ":" + port);
}

void ssl_write_all(SSL* ssl, const unsigned char* data, size_t size) {
  size_t offset = 0;
  while (offset < size) {
    const int written = SSL_write(ssl, data + offset, static_cast<int>(size - offset));
    if (written <= 0) {
      throw std::runtime_error("SSL_write failed");
    }
    offset += static_cast<size_t>(written);
  }
}

void ssl_read_all(SSL* ssl, unsigned char* data, size_t size) {
  size_t offset = 0;
  while (offset < size) {
    const int read = SSL_read(ssl, data + offset, static_cast<int>(size - offset));
    if (read <= 0) {
      throw std::runtime_error("SSL_read failed");
    }
    offset += static_cast<size_t>(read);
  }
}

void send_frame(SSL* ssl, unsigned char opcode, const std::string& payload) {
  std::vector<unsigned char> frame;
  frame.push_back(static_cast<unsigned char>(0x80 | opcode));

  const size_t size = payload.size();
  if (size <= 125) {
    frame.push_back(static_cast<unsigned char>(0x80 | size));
  } else if (size <= 0xffff) {
    frame.push_back(0x80 | 126);
    frame.push_back(static_cast<unsigned char>((size >> 8) & 0xff));
    frame.push_back(static_cast<unsigned char>(size & 0xff));
  } else {
    frame.push_back(0x80 | 127);
    for (int i = 7; i >= 0; --i) {
      frame.push_back(static_cast<unsigned char>((size >> (i * 8)) & 0xff));
    }
  }

  unsigned char mask[4];
  RAND_bytes(mask, sizeof(mask));
  frame.insert(frame.end(), mask, mask + sizeof(mask));
  for (size_t i = 0; i < payload.size(); ++i) {
    frame.push_back(static_cast<unsigned char>(payload[i]) ^ mask[i % 4]);
  }

  ssl_write_all(ssl, frame.data(), frame.size());
}

std::pair<unsigned char, std::string> read_frame(SSL* ssl) {
  unsigned char header[2];
  ssl_read_all(ssl, header, 2);

  const unsigned char opcode = header[0] & 0x0f;
  const bool masked = (header[1] & 0x80) != 0;
  uint64_t size = header[1] & 0x7f;
  if (size == 126) {
    unsigned char extended[2];
    ssl_read_all(ssl, extended, 2);
    size = (static_cast<uint64_t>(extended[0]) << 8) | extended[1];
  } else if (size == 127) {
    unsigned char extended[8];
    ssl_read_all(ssl, extended, 8);
    size = 0;
    for (unsigned char byte : extended) {
      size = (size << 8) | byte;
    }
  }

  unsigned char mask[4] = {0, 0, 0, 0};
  if (masked) {
    ssl_read_all(ssl, mask, 4);
  }

  std::string payload(size, '\0');
  if (size > 0) {
    ssl_read_all(ssl, reinterpret_cast<unsigned char*>(payload.data()), static_cast<size_t>(size));
  }
  if (masked) {
    for (size_t i = 0; i < payload.size(); ++i) {
      payload[i] = static_cast<char>(static_cast<unsigned char>(payload[i]) ^ mask[i % 4]);
    }
  }
  return {opcode, payload};
}

}  // namespace

int run_public_ws(const std::string& symbol, const std::string& topic_id, int local_no) {
  const std::string host = "stream.deepcoin.com";
  const std::string port = "443";
  const std::string target = "/streamlet/trade/public/swap?platform=api";

  SSL_library_init();
  SSL_load_error_strings();
  const SSL_METHOD* method = TLS_client_method();
  SSL_CTX* ctx = SSL_CTX_new(method);
  if (ctx == nullptr) {
    std::cerr << "SSL_CTX_new failed\n";
    return 1;
  }

  int raw_fd = -1;
  SSL* ssl = nullptr;
  try {
    raw_fd = connect_tcp(host, port);
    ssl = SSL_new(ctx);
    SSL_set_fd(ssl, raw_fd);
    SSL_set_tlsext_host_name(ssl, host.c_str());
    if (SSL_connect(ssl) != 1) {
      throw std::runtime_error("SSL_connect failed");
    }

    unsigned char nonce[16];
    RAND_bytes(nonce, sizeof(nonce));
    const std::string key = base64(nonce, sizeof(nonce));
    const std::string upgrade =
        "GET " + target + " HTTP/1.1\r\n"
        "Host: " + host + "\r\n"
        "Upgrade: websocket\r\n"
        "Connection: Upgrade\r\n"
        "Sec-WebSocket-Key: " + key + "\r\n"
        "Sec-WebSocket-Version: 13\r\n\r\n";
    ssl_write_all(ssl, reinterpret_cast<const unsigned char*>(upgrade.data()), upgrade.size());

    std::string response;
    char ch = '\0';
    while (response.find("\r\n\r\n") == std::string::npos) {
      ssl_read_all(ssl, reinterpret_cast<unsigned char*>(&ch), 1);
      response.push_back(ch);
    }
    if (response.find(" 101 ") == std::string::npos) {
      throw std::runtime_error("websocket upgrade failed:\n" + response);
    }

    const std::string subscribe =
        R"({"SendTopicAction":{"Action":"1","FilterValue":")" + symbol +
        R"(","LocalNo":)" + std::to_string(local_no) +
        R"(,"ResumeNo":-2,"TopicID":")" + topic_id + R"("}})";

    send_frame(ssl, 0x1, subscribe);
    std::cout << "Sent: " << subscribe << std::endl;

    send_frame(ssl, 0x1, "ping");
    std::cout << "Sent: ping" << std::endl;

    bool running = true;
    while (running) {
      auto [opcode, payload] = read_frame(ssl);
      if (opcode == 0x1 || opcode == 0x2) {
        std::cout << "Received: " << payload << std::endl;
      } else if (opcode == 0x8) {
        running = false;
      } else if (opcode == 0x9) {
        send_frame(ssl, 0xA, payload);
      }
    }
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    if (ssl != nullptr) {
      SSL_free(ssl);
    }
    if (raw_fd >= 0) {
      close(raw_fd);
    }
    SSL_CTX_free(ctx);
    return 1;
  }

  SSL_free(ssl);
  close(raw_fd);
  SSL_CTX_free(ctx);
  return 0;
}

}  // namespace deepcoin
