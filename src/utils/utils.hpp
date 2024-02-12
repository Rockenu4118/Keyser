#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <nlohmann/json.hpp>

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
        void encodeJson(nlohmann::json& json, Transaction& transaction);
        void decodeJson(Transaction& transaction, nlohmann::json& json);

        // Block Encoding/Decoding
        void encodeJson(nlohmann::json& json, Block& block);
        void decodeJson(Block& block, nlohmann::json& json);

        // NodeInfo Encoding/Decoding
        void encodeJson(nlohmann::json&, NodeInfo& nodeInfo);
        void decodeJson(NodeInfo& nodeInfo, nlohmann::json& json);

        // Time functions
        std::string localTimestamp();
        std::string localTime(time_t time);

    }
}

#endif