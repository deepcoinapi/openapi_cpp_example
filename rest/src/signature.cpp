#include "deepcoin/signature.h"

#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/evp.h>
#include <openssl/hmac.h>

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <stdexcept>

namespace deepcoin {

std::string utc_timestamp_ms() {
  using namespace std::chrono;
  const auto now = system_clock::now();
  const auto millis = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;
  const std::time_t seconds = system_clock::to_time_t(now);

  std::tm utc{};
#if defined(_WIN32)
  gmtime_s(&utc, &seconds);
#else
  gmtime_r(&seconds, &utc);
#endif

  std::ostringstream out;
  out << std::put_time(&utc, "%Y-%m-%dT%H:%M:%S") << '.'
      << std::setw(3) << std::setfill('0') << millis.count() << 'Z';
  return out.str();
}

std::string hmac_sha256_base64(const std::string& secret, const std::string& message) {
  unsigned int digest_len = EVP_MAX_MD_SIZE;
  unsigned char digest[EVP_MAX_MD_SIZE];

  if (HMAC(EVP_sha256(),
           secret.data(),
           static_cast<int>(secret.size()),
           reinterpret_cast<const unsigned char*>(message.data()),
           message.size(),
           digest,
           &digest_len) == nullptr) {
    throw std::runtime_error("HMAC-SHA256 failed");
  }

  BIO* b64 = BIO_new(BIO_f_base64());
  BIO* mem = BIO_new(BIO_s_mem());
  BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
  BIO_push(b64, mem);
  BIO_write(b64, digest, static_cast<int>(digest_len));
  BIO_flush(b64);

  BUF_MEM* buffer = nullptr;
  BIO_get_mem_ptr(b64, &buffer);
  std::string encoded(buffer->data, buffer->length);
  BIO_free_all(b64);
  return encoded;
}

std::string sign_request(const std::string& timestamp,
                         const std::string& method,
                         const std::string& request_path,
                         const std::string& body,
                         const std::string& secret) {
  return hmac_sha256_base64(secret, timestamp + method + request_path + body);
}

}  // namespace deepcoin
