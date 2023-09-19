#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <boost/asio.hpp>

#include "./tsqueue.hpp"
#include "./connection.hpp"
#include "./net_message.hpp"


namespace networking
{
    template <typename T>
    class Client
    {
        public:
            Client() = default;
            virtual ~Client();

            bool connect(const std::string& host, const uint16_t port);
            void disconnect();
            bool isConnected();

            tsqueue<OwnedMessage<T>>& incoming();

        protected:
            // context handles data transfer
            boost::asio::io_context _context;
            // Needs to run in its own thread to execute commands
            std::thread _thrContext;
            // Physical socket connected to the server
            boost::asio::ip::tcp::socket _socket;
            // Client has a single connection object to handle data transfer
            std::unique_ptr<Connection<T>> _connection;


        private:
            // Thread safe queue for incoming messages from server
            tsqueue<OwnedMessage<T>> _qMessagesIn;
    };
}

#endif