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
#include "../chain/Chain.hpp"
#include "../wallet/WalletManager.hpp"
#include "../chain/Mempool.hpp"
#include "../storage/StorageEngine.hpp"



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
            void managePeerConnections();
            void removeInvalidConnections();

            // Handle incoming messages
            void update(uint8_t maxMessages = -1, bool wait = true);

            // Initial msg sent to establish handshake from a new node
            void version(std::shared_ptr<Connection> connection);

            void displayConnections();
            void displayActiveNodes();
            void displaySelfInfo();

            Chain*         chain();
            Mempool*       mempool();
            StorageEngine* storageEngine();
            WalletManager* walletManager();

        protected:
            virtual bool allowConnect(std::shared_ptr<Connection> connection);
            virtual void onOutgoingConnect(std::shared_ptr<Connection> connection);
            virtual void onIncomingConnect(std::shared_ptr<Connection> connection);
            virtual void onDisconnect(std::shared_ptr<Connection> connection);
            virtual void onMessage(std::shared_ptr<Connection> connection, Message& msg);

            // Node members
            Chain*         _chain = nullptr;
            Mempool*       _mempool = nullptr;
            StorageEngine* _storageEngine = nullptr;
            WalletManager* _walletManager = nullptr;

            std::thread _miningThr;
            bool        _miningStatus = false;

            // Thread safe queue for incoming messages
            tsqueue<OwnedMessage> _messagesIn;
            
            // Thread for handling incoming messages
            std::thread _responseThread;

            // Container for active validated connections
            std::deque<std::shared_ptr<Connection>> _connections{};

            // Info of active nodes on network, info of connected nodes, self info
            std::set<NodeInfo> _activeNodeList;
            std::set<NodeInfo> _connectedNodeList;
            NodeInfo           _selfInfo;
            bool               _recievedNodeList = false;
            bool               _recievedChain = false;

            // Asio context as well as its own thread to run in
            boost::asio::io_context _context;
            std::thread             _contextThread;

            // Thread for cleaning invalid connections
            // Mutex and condition variable for blocking thread
            // until connection either fails or disconnects
            std::condition_variable _cvBlocking;
            std::mutex              _muxBlocking;
            std::thread             _connectionRemovalThread;

            // Thread for handling peer connections
            std::thread _peerConnectionThread;
            std::mutex _muxConnections;
            std::condition_variable _cvConnections;

            // Asio acceptor
            boost::asio::ip::tcp::acceptor _acceptor;

            // Connections will be identified in the system by an id
            uint16_t _idCounter = 10000;
    };
}

#endif