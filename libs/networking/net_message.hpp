#ifndef NET_MESSAGE_H
#define NET_MESSAGE_H

#include <iostream>
#include <vector>
#include <memory>
#include <cstring>

#include "./Connection.hpp"




namespace networking
{
    // Forwrard declare 
    template <typename T>
    class Connection;

    template <typename T>
    class MessageHeader
    {
        public:
            // Constructor
            MessageHeader() = default;

            // Members
            T        id{};
            uint32_t size = 0;
    };


    template <typename T>
    class Message
    {
        friend std::ostream& operator<<(std::ostream& out, const Message<T>& msg) {
            out << "ID: " << int(msg.header.id) << ", Size: " << msg.header.size << std::endl;
            return out;
        }

        template <typename DataType>
        friend Message<T>& operator<<(Message<T>& msg, const DataType& data)
        {
            static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to push into vector");

            size_t i = msg.body.size();

            msg.body.resize(msg.body.size() + sizeof(DataType));

            std::memcpy(msg.body.data() + i, &data, sizeof(DataType));

            msg.header.size = msg.size();

            return msg;
        }

        template <typename DataType>
        friend Message<T>& operator>>(Message<T>& msg, DataType& data)
        {
            static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pulled from vector");

            size_t i = msg.body.size() - sizeof(DataType);

            std::memcpy(&data, msg.body.data() + i, sizeof(DataType));

            msg.body.resize(i);

            msg.header.size = msg.size();

            return msg;
        }


        public:
            // Constructor
            Message() = default;

            // Utility
            size_t size() const
            {
                return body.size();
            }

            // Members
            MessageHeader<T>     header{};
            std::vector<uint8_t> body;
    };

    template <typename T>
    class OwnedMessage
    {
        friend std::ostream& operator<<(std::ostream& out, const OwnedMessage<T>& msg) {
            out << msg.msg;
            return out;
        }

        public:
            // Constructor
            OwnedMessage(std::shared_ptr<Connection<T>> remote, Message<T> msg)
            {
                _remote = remote;
                _msg    = msg;
            }

            // Members
            std::shared_ptr<Connection<T>> _remote = nullptr;
            Message<T>                     _msg;

    };
}

#endif