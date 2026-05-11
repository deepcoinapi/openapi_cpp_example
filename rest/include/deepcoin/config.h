#pragma once

#include <string>

namespace deepcoin {

struct Config {
  std::string host = "https://api.deepcoin.com";
  std::string api_key;
  std::string secret_key;
  std::string passphrase;
};

Config load_config(const std::string& path);

}  // namespace deepcoin
