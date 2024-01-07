#ifndef NODE_INTERFACE_H
#define NODE_INTERFACE_H

#include <set>
#include <string>
#include <mutex>
#include <condition_variable>
#include <boost/asio.hpp>
#include <tsqueue.hpp>

#include "../net/Connection.hpp"
#include "../net/Message.hpp"
#include "../node/NodeInfo.hpp"


namespace keyser
{
    class Connection;

    class Node_Interface
    {
        public:
            Node_Interface(uint16_t port);
            
            virtual ~Node_Interface();

            bool start();
            bool connect(const NodeInfo nodeInfo);
            void shutdown();
            void waitForConnection();
            void message(std::shared_ptr<Connection> connection, const Message& msg);
            void messageNeighbors(const Message& msg, std::shared_ptr<Connection> ignoreConnection = nullptr);
            void removeInvalidConnections();

            // Handle incoming messages
            void update(uint8_t maxMessages = -1, bool wait = true);

            void displayConnections();
            void displayActiveNodes();

        protected:
            virtual void onOutgoingConnect(std::shared_ptr<Connection> connection);
            virtual bool allowConnect(std::shared_ptr<Connection> connection);
            virtual void onIncomingConnect(std::shared_ptr<Connection> connection);
            virtual void onDisconnect(std::shared_ptr<Connection> connection);
            virtual void onMessage(std::shared_ptr<Connection> connection, Message& msg);

            // Thread safe queue for incoming messages
            tsqueue<OwnedMessage> _messagesIn;
            
            // Thread for handling incoming messages
            std::thread _responseThread;

            // Container for active validated connections
            std::deque<std::shared_ptr<Connection>> _connections{};

            // Addresses of active nodes on network
            std::set<NodeInfo> _activeNodeList;

            // Asio context as well as its own thread to run in
            boost::asio::io_context _context;
            std::thread             _contextThread;

            // Thread for cleaning invalid connections
            // Mutex and condition variable for blocking thread
            // until connection either fails or disconnects
            std::condition_variable _cvBlocking;
            std::mutex              _muxBlocking;
            std::thread             _connectionRemovalThread;

            // Asio acceptor
            boost::asio::ip::tcp::acceptor _acceptor;

            // Current port server is running on
            uint16_t _port;

            // Connections will be identified in the system by an id
            uint16_t _idCounter = 10000;
    };
}

#endif