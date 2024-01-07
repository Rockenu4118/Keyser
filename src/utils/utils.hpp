#ifndef UTILS_H
#define UTILS_H

#include <string>

#include "../chain/Transaction.hpp"
#include "../chain/Block.hpp"
#include "../node/NodeInfo.hpp"

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

        // NodeInfo Encoding/Decoding
        void encodeJson(std::string& jsonStr, NodeInfo& nodeInfo);
        void decodeJson(NodeInfo& nodeInfo, std::string& jsonStr);        

        std::string localTimestamp();

    }
}

#endif