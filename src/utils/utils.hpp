#ifndef UTILS_H
#define UTILS_H

#include <string>

#include "../chain/Transaction.hpp"
#include "../chain/Block.hpp"

namespace keyser
{
    namespace utils
    {
        std::string pubKeytoAddress(const std::string& uPublicKey);
        std::string hexToString(const std::string& input);
        
        void encodeJson(std::string& jsonStr, Transaction& transaction);
        void decodeJson(Transaction& transaction, std::string& jsonStr);

        void encodeJson(std::string& jsonStr, Block& block);
        void decodeJson(Block& block, std::string& jsonStr);
    }
}

#endif