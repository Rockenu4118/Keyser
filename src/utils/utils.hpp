#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <set>

#include "../chain/Transaction.hpp"
#include "../chain/Block.hpp"

namespace keyser
{
    namespace utils
    {
        std::string pubKeytoAddress(const std::string& uPublicKey);
        std::string hexToString(const std::string& input);
        
        // Transaction Encoding/Decoding
        void encodeJson(std::string& jsonStr, Transaction& transaction);
        void decodeJson(Transaction& transaction, std::string& jsonStr);

        // Block Encoding/Decoding
        void encodeJson(std::string& jsonStr, Block& block);
        void decodeJson(Block& block, std::string& jsonStr);

        // Addr List Encoding/Decoding
        

        std::string localTimestamp();

    }
}

#endif