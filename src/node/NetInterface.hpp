#ifndef NET_INTERFACE_H
#define NET_INTERFACE_H

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

    class NetInterface
    {
        public:
            NetInterface(uint16_t port);
            
            virtual ~NetInterface();

            bool startServer();
            bool connect(const NodeInfo nodeInfo);
            void shutdown();
            void acceptConnection();
            void message(std::shared_ptr<Connection> connection, const Message& msg);
            void messageNeighbors(const Message& msg, std::shared_ptr<Connection> ignoreConnection = nullptr);
            void managePeerConnections();
            void removeInvalidConnections();

            void addUserProvidedPotentialConnection(NodeInfo nodeInfo);
            void addPotentialConnection(NodeInfo nodeInfo);

            // Handle incoming messages
            void update(uint8_t maxMessages = -1, bool wait = true);

            // Accessors
            int connectionCount() const;

            void displayConnections();
            void displayActiveNodes();
            void displaySelfInfo();

        protected:
            virtual bool allowConnect(std::shared_ptr<Connection> connection);
            virtual void onOutgoingConnect(std::shared_ptr<Connection> connection);
            virtual void onIncomingConnect(std::shared_ptr<Connection> connection);
            virtual void onDisconnect(std::shared_ptr<Connection> connection);
            virtual void onMessage(std::shared_ptr<Connection> connection, Message& msg);


            // Thread safe queue for incoming messages
            tsqueue<OwnedMessage> _messagesIn;
            
            // Thread for handling incoming messages
            std::thread _responseThread;

            // Container for active validated connections
            std::deque<std::shared_ptr<Connection>> _connections{};

            // Container for info of potential connections
            std::deque<NodeInfo> _potentialConnections{};

            // Info of active nodes on network, info of connected nodes, self info
            std::set<NodeInfo> _activeNodeList;
            std::set<NodeInfo> _connectedNodeList;
            NodeInfo           _selfInfo;
            
            // Status variables
            bool _recievedNodeList       = false;
            bool _completedPeerDiscovery = false;
            bool _distributedSelfInfo    = false;
            bool _recievedChain          = false;

            // Asio context as well as its own thread to run in
            boost::asio::io_context _context;
            std::thread             _contextThread;

            // Thread for cleaning invalid connections
            // Mutex and condition variable for blocking thread
            // until connection either fails or disconnects
            std::condition_variable _cvBlocking;
            std::mutex              _muxBlocking;
            std::thread             _connectionRemovalThread;

            std::condition_variable _connectionBlocking;
            std::mutex              _connectionMutex;

            // Thread for handling peer connections
            std::thread _peerConnectionThread;

            // Asio acceptor
            boost::asio::ip::tcp::acceptor _acceptor;

            // Connections will be identified in the system by an id
            uint16_t _idCounter = 10000;
    };
}

#endif