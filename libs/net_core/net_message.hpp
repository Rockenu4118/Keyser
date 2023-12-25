#ifndef NET_MESSAGE_H
#define NET_MESSAGE_H

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <cstring>

#include "./Connection.hpp"
#include "./Net_Connection.hpp"


namespace net_core
{
    // Forwrard declare 
    template <typename T>
    class Net_Connection;

    template <typename T>
    struct MessageHeader
    {
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
            Message() = default;

            size_t size() const
            {
                return body.size();
            }

            void print()
            {
                for (int i = 0 ; i < header.size ; i++)
                {
                    std::cout << body.at(i);
                }

                std::cout << std::endl;
            }

            void push(const std::string& msg)
            {
                int len = msg.size();

                for (int i = 0 ; i < len ; i++)
                {
                    body.push_back(msg[i]);
                }

                header.size = size();
            }

            void pull(std::string& msg)
            {
                int len = body.size();

                for (int i = 0 ; i < len ; i++)
                {
                    msg += body.at(i);
                }
            }

            MessageHeader<T>  header{};
            std::vector<char> body;
    };

    template <typename T>
    class OwnedMessage
    {
        friend std::ostream& operator<<(std::ostream& out, const OwnedMessage<T>& msg) {
            out << msg._msg;
            return out;
        }

        public:
            OwnedMessage(std::shared_ptr<Net_Connection<T>> remote, Message<T> msg)
            {
                _remote = remote;
                _msg    = msg;
            }

            std::shared_ptr<Net_Connection<T>> _remote = nullptr;
            Message<T>                         _msg;
    };
}

#endif