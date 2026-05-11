#pragma once

#include <string>

namespace deepcoin {

std::string utc_timestamp_ms();
std::string hmac_sha256_base64(const std::string& secret, const std::string& message);
std::string sign_request(const std::string& timestamp,
                         const std::string& method,
                         const std::string& request_path,
                         const std::string& body,
                         const std::string& secret);

}  // namespace deepcoin
