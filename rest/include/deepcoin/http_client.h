#pragma once

#include "deepcoin/config.h"

#include <string>
#include <utility>
#include <vector>

namespace deepcoin {

using Params = std::vector<std::pair<std::string, std::string>>;

struct HttpResponse {
  long status = 0;
  std::string body;
};

class HttpClient {
 public:
  explicit HttpClient(Config config);

  HttpResponse get(const std::string& path, const Params& params = {}, bool auth = true) const;
  HttpResponse post(const std::string& path, const std::string& json_body, bool auth = true) const;

  static std::string query_string(const Params& params);

 private:
  HttpResponse request(const std::string& method,
                       const std::string& request_path,
                       const std::string& body,
                       bool auth) const;

  Config config_;
};

}  // namespace deepcoin
