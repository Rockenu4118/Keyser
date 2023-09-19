#ifndef MESSAGE_H
#define MESSAGE_H

#include <iostream>
#include <vector>
#include <memory>
#include <cstring>

#include "./connection.hpp"

// Forwrard declare
template <typename T>
class Connection;


namespace networking
{
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

        public:
            // Constructor
            Message() = default;

            // Utility
            size_t size() const;

            // Members
            MessageHeader<T>     header{};
            std::vector<uint8_t> body;
    };

    template <typename T>
    class OwnedMessage
    {
        template <typename U>
        friend std::ostream& operator<<(std::ostream& out, const OwnedMessage<U>& msg) {
            out << msg.msg;
            return out;
        }

        public:
            // Constructor
            OwnedMessage() = default;

            // Members
            std::shared_ptr<Connection<T>> remote = nullptr;
            Message<T> msg;

    };
}







#endif