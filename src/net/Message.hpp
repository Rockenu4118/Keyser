#ifndef MESSAGE_H
#define MESSAGE_H

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <cstring>

#include "./Connection.hpp"
#include "./MsgTypes.hpp"
#include "../chain/Block.hpp"
#include "../chain/Transaction.hpp"
#include "../node/NodeInfo.hpp"


namespace keyser
{
    // Forwrard declare 
    class Connection;

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

        private:
            void push(const std::string& msg);
            void pull(std::string& msg);

        public:
            void insert(Block& block);
            void insert(Transaction& transaction);
            void insert(NodeInfo& NodeInfo);

            void extract(Block& block);
            void extract(Transaction& transaction);
            void extract(NodeInfo& nodeInfo);

            size_t size() const;
            void print();
            

            MessageHeader     header{};
            std::vector<char> body;
    };

    struct OwnedMessage
    {
        friend std::ostream& operator<<(std::ostream& out, const OwnedMessage& msg) {
            out << msg._msg;
            return out;
        }

        OwnedMessage(std::shared_ptr<Connection> remote, Message msg)
        {
            _remote = remote;
            _msg    = msg;
        }

        std::shared_ptr<Connection> _remote = nullptr;
        Message                     _msg;
    };
}

#endif