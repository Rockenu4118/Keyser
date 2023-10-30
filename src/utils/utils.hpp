#ifndef UTILS_H
#define UTILS_H

#include <string>

#include "../chain/Transaction.hpp"

namespace keyser
{
    namespace utils
    {
        std::string pubKeytoAddress(const std::string& uPublicKey);
        std::string hexToString(const std::string& input);
        
        std::string encodeJson(Transaction& transaction);

        Transaction decodeJson(std::string jsonStr);
    }
}

#endif