#include "deepcoin/http_client.h"

#include "deepcoin/signature.h"

#include <curl/curl.h>

#include <iostream>
#include <sstream>
#include <stdexcept>

namespace deepcoin {
namespace {

size_t write_body(char* ptr, size_t size, size_t nmemb, void* userdata) {
  auto* body = static_cast<std::string*>(userdata);
  body->append(ptr, size * nmemb);
  return size * nmemb;
}

std::string curl_escape(CURL* curl, const std::string& value) {
  char* escaped = curl_easy_escape(curl, value.c_str(), static_cast<int>(value.size()));
  if (escaped == nullptr) {
    throw std::runtime_error("curl_easy_escape failed");
  }
  std::string result(escaped);
  curl_free(escaped);
  return result;
}

}  // namespace

HttpClient::HttpClient(Config config) : config_(std::move(config)) {
  curl_global_init(CURL_GLOBAL_DEFAULT);
}

HttpResponse HttpClient::get(const std::string& path, const Params& params, bool auth) const {
  const std::string query = query_string(params);
  return request("GET", path + query, "", auth);
}

HttpResponse HttpClient::post(const std::string& path, const std::string& json_body, bool auth) const {
  return request("POST", path, json_body, auth);
}

std::string HttpClient::query_string(const Params& params) {
  if (params.empty()) {
    return "";
  }

  CURL* curl = curl_easy_init();
  if (curl == nullptr) {
    throw std::runtime_error("curl_easy_init failed");
  }

  std::ostringstream out;
  out << '?';
  for (size_t i = 0; i < params.size(); ++i) {
    if (i > 0) {
      out << '&';
    }
    out << curl_escape(curl, params[i].first) << '=' << curl_escape(curl, params[i].second);
  }
  curl_easy_cleanup(curl);
  return out.str();
}

HttpResponse HttpClient::request(const std::string& method,
                                 const std::string& request_path,
                                 const std::string& body,
                                 bool auth) const {
  CURL* curl = curl_easy_init();
  if (curl == nullptr) {
    throw std::runtime_error("curl_easy_init failed");
  }

  std::string response_body;
  const std::string url = config_.host + request_path;
  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_body);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_body);
  curl_easy_setopt(curl, CURLOPT_USERAGENT, "deepcoin-openapi-cpp-example/1.0");
  curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);

  curl_slist* headers = nullptr;
  headers = curl_slist_append(headers, "Content-Type: application/json");

  if (auth) {
    const std::string timestamp = utc_timestamp_ms();
    const std::string signature = sign_request(timestamp, method, request_path, body, config_.secret_key);
    headers = curl_slist_append(headers, ("DC-ACCESS-KEY: " + config_.api_key).c_str());
    headers = curl_slist_append(headers, ("DC-ACCESS-SIGN: " + signature).c_str());
    headers = curl_slist_append(headers, ("DC-ACCESS-TIMESTAMP: " + timestamp).c_str());
    headers = curl_slist_append(headers, ("DC-ACCESS-PASSPHRASE: " + config_.passphrase).c_str());
  }

  if (method == "POST") {
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, body.size());
  }

  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
  const CURLcode code = curl_easy_perform(curl);

  long status = 0;
  curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &status);
  curl_slist_free_all(headers);
  curl_easy_cleanup(curl);

  if (code != CURLE_OK) {
    throw std::runtime_error(curl_easy_strerror(code));
  }
  return HttpResponse{status, response_body};
}

}  // namespace deepcoin
