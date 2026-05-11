#pragma once

#include "deepcoin/http_client.h"

#include <functional>
#include <map>
#include <string>
#include <vector>

namespace deepcoin {

using Command = std::function<int()>;

class ExampleRegistry {
 public:
  explicit ExampleRegistry(HttpClient client);

  int run(const std::string& command) const;
  void print_commands() const;
  std::vector<std::string> names() const;

 private:
  void add(const std::string& name, Command command);
  int print_response(const HttpResponse& response) const;

  HttpClient client_;
  std::map<std::string, Command> commands_;
};

}  // namespace deepcoin
