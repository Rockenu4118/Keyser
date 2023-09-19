#ifndef SERVER_H
#define SERVER_H

#include <memory>
#include <deque>
#include <boost/asio.hpp>

#include "./connection.hpp"
#include "./net_message.hpp"
#include "./tsqueue.hpp"

namespace networking
{
    template <typename T>
    class Server
    {
        public:
            Server(uint16_t port);
            virtual ~Server();

            bool start();
            void stop();

            // Async
            void waitForClientConnection();

            // Send message to specific client
            void messageClient(std::shared_ptr<Connection<T>> client, const Message<T>& msg);

            // Send message to all clients
            void messageAllClients(const Message<T>& msg, std::shared_ptr<Connection<T>> ignoreClient = nullptr);

            // Allow user to explicitly limit amount of messages allowed to be handled at once
            void update(size_t maxMessages = -1);

        protected:
            virtual bool onClientConnect(std::shared_ptr<Connection<T>> client);

            virtual void onClientDisconnect(std::shared_ptr<Connection<T>> client);

            virtual void onMessage(std::shared_ptr<Connection<T>> client, Message<T>& msg);

            // Thread safe queue for incoming messages
            tsqueue<OwnedMessage<T>> _qMessagesIn;

            // Container for active validated connections
            std::deque<std::shared_ptr<Connection<T>>> _deqConnections;

            // Asio context as well as its own thread to run in
            boost::asio::io_context _context;
            std::thread             _thrContext;

            // Asio acceptor
            boost::asio::ip::tcp::acceptor _acceptor;

            // Clients will be identified in the system by an id
            uint32_t _idCounter = 10000;

    };
}

#endif