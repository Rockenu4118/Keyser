#ifndef MESSAGE_H
#define MESSAGE_H

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <cstring>
#include <nlohmann/json.hpp>

#include "./MsgTypes.hpp"
#include "../chain/Block.hpp"
#include "../chain/Transaction.hpp"
#include "../node/NodeInfo.hpp"



namespace keyser
{
    struct MessageHeader
    {
        MsgTypes id = MsgTypes::Generic;
        uint32_t size = 0;
    };

    class Message
    {
        friend std::ostream& operator<<(std::ostream& out, const Message& msg) {
            out << "ID: " << int(msg.header.id) << ", Size: " << msg.header.size << std::endl;
            return out;
        }

        public:
            Message() = default;
            Message(MsgTypes id);

            void serialize();
            void deserialize();

            nlohmann::json& json() { return _doc; }

            void insert(Block& block);
            void insert(Transaction& transaction);
            void insert(NodeInfo& NodeInfo);

            void extract(Block& block);
            void extract(Transaction& transaction);
            void extract(NodeInfo& nodeInfo);

            size_t size() const;
            void   print();
            
            nlohmann::json    _doc;
            MessageHeader     header{};
            std::vector<char> body;
    };
}

#endif