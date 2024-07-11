#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <nlohmann/json.hpp>

#include "../wallet/Wallet.hpp"
#include "../chain/Transaction.hpp"
#include "../chain/Block.hpp"
#include "../node/PeerInfo.hpp"

namespace keyser
{
    namespace utils
    {
        std::string pubKeytoAddress(const std::string& uPublicKey);
        std::string hexToString(const std::string& input);
        
        bool isValidHash(std::string hash, uint8_t difficulty);

        // Time functions
        std::string localTimestamp();
        std::string localTime(time_t time);

    }
}

#endif