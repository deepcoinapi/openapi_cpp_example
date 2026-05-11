#pragma once

#include <string>

namespace deepcoin {

int run_public_ws(const std::string& symbol = "DeepCoin_ETHUSDT",
                  const std::string& topic_id = "2",
                  int local_no = 111);

}  // namespace deepcoin
