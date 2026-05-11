#include "deepcoin/ws_public.h"

#include <exception>
#include <iostream>
#include <string>

int main(int argc, char** argv) {
  try {
    std::string symbol = "DeepCoin_ETHUSDT";
    std::string topic_id = "2";
    int local_no = 111;

    if (argc > 1 && (std::string(argv[1]) == "help" || std::string(argv[1]) == "--help")) {
      std::cerr << "Usage: ./build/deepcoin_ws [symbol] [topic_id] [local_no]\n"
                << "Example: ./build/deepcoin_ws DeepCoin_ETHUSDT 2 111\n";
      return 0;
    }
    if (argc > 1) {
      symbol = argv[1];
    }
    if (argc > 2) {
      topic_id = argv[2];
    }
    if (argc > 3) {
      local_no = std::stoi(argv[3]);
    }

    return deepcoin::run_public_ws(symbol, topic_id, local_no);
  } catch (const std::exception& e) {
    std::cerr << "error: " << e.what() << '\n';
    return 1;
  }
}
