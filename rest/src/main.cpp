#include "deepcoin/config.h"
#include "deepcoin/examples.h"
#include "deepcoin/http_client.h"

#include <exception>
#include <iostream>
#include <string>

int main(int argc, char** argv) {
  try {
    const deepcoin::Config config = deepcoin::load_config("rest/config/master.yaml");
    deepcoin::ExampleRegistry registry{deepcoin::HttpClient(config)};

    if (argc < 2 || std::string(argv[1]) == "help" || std::string(argv[1]) == "--help") {
      std::cerr << "Usage: ./build/deepcoin_rest <command>\n\n";
      registry.print_commands();
      return argc < 2 ? 1 : 0;
    }

    return registry.run(argv[1]);
  } catch (const std::exception& e) {
    std::cerr << "error: " << e.what() << '\n';
    return 1;
  }
}
