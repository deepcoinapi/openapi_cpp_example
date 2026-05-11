#include "deepcoin/config.h"

#include <cstdlib>
#include <fstream>
#include <sstream>

namespace deepcoin {
namespace {

std::string trim(std::string value) {
  const auto first = value.find_first_not_of(" \t\r\n");
  if (first == std::string::npos) {
    return "";
  }
  const auto last = value.find_last_not_of(" \t\r\n");
  value = value.substr(first, last - first + 1);
  if (value.size() >= 2 && value.front() == '"' && value.back() == '"') {
    value = value.substr(1, value.size() - 2);
  }
  return value;
}

std::string env_or_empty(const char* name) {
  const char* value = std::getenv(name);
  return value == nullptr ? "" : value;
}

void set_from_env(Config& config) {
  const std::string host = env_or_empty("DEEPCOIN_API_URL");
  const std::string key = env_or_empty("DEEPCOIN_API_KEY");
  const std::string secret = env_or_empty("DEEPCOIN_SECRET_KEY");
  const std::string passphrase = env_or_empty("DEEPCOIN_PASSPHRASE");

  if (!host.empty()) config.host = host;
  if (!key.empty()) config.api_key = key;
  if (!secret.empty()) config.secret_key = secret;
  if (!passphrase.empty()) config.passphrase = passphrase;
}

}  // namespace

Config load_config(const std::string& path) {
  Config config;
  std::ifstream input(path);
  std::string line;
  while (std::getline(input, line)) {
    const auto comment = line.find('#');
    if (comment != std::string::npos) {
      line = line.substr(0, comment);
    }
    const auto colon = line.find(':');
    if (colon == std::string::npos) {
      continue;
    }
    const std::string key = trim(line.substr(0, colon));
    const std::string value = trim(line.substr(colon + 1));
    if (key == "url" || key == "host") {
      config.host = value;
    } else if (key == "key" || key == "api_key") {
      config.api_key = value;
    } else if (key == "secret_key") {
      config.secret_key = value;
    } else if (key == "passphrase") {
      config.passphrase = value;
    }
  }

  set_from_env(config);
  return config;
}

}  // namespace deepcoin
