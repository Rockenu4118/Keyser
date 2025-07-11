#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <nlohmann/json.hpp>

#include "../wallet/Wallet.hpp"
// #include "../chain/Transaction.hpp"
#include "../primitives/Block.hpp"
#include "../node/PeerInfo.hpp"

namespace keyser
{
    namespace utils
    {
        std::vector<std::string> parse_cmd(const std::string input);

        std::string pubKeytoAddress(const std::string& uPublicKey);
        std::string hexToString(const std::string& input);
        

        // Time functions
        std::string localTimestamp();
        std::string localTime(time_t time);
    }
}

#endif