#ifndef NET_INTERFACE_H
#define NET_INTERFACE_H

#include <set>
#include <vector>
#include <string>
#include <mutex>
#include <condition_variable>
#include <boost/asio.hpp>
#include <tsqueue.hpp>

#include "./Peer.hpp"
#include "../net/Message.hpp"
#include "../node/NodeInfo.hpp"
#include "../chain/Chain.hpp"
#include "../wallet/WalletManager.hpp"
#include "../chain/Mempool.hpp"
#include "../storage/StorageEngine.hpp"


namespace keyser
{
    class Peer;

    class NetInterface
    {
        public:
            NetInterface(uint16_t port);
            
            virtual ~NetInterface();

            bool startServer();
            bool connect(const NodeInfo nodeInfo);
            void shutdown();
            void acceptConnection();
            void message(std::shared_ptr<Peer> peer, const Message& msg);
            void messageNeighbors(const Message& msg, std::shared_ptr<Peer> ignorePeer = nullptr);
            void managePeers();

            void addUserProvidedPotentialConnection(NodeInfo nodeInfo);
            void addPotentialConnection(NodeInfo nodeInfo);

            // Handle incoming messages
            void update(uint8_t maxMessages = -1, bool wait = true);

            // Accessors
            int connectionCount() const;
            
            std::shared_ptr<Peer> syncNode();

            void displayConnections();
            void displayActiveNodes();
            void displaySelfInfo();

            std::vector<NodeInfo> getConnections() const;

            bool validateConnection(std::shared_ptr<Peer>& connection);

        protected:
            virtual bool allowConnect(std::shared_ptr<Peer> peer);
            virtual void onOutgoingConnect(std::shared_ptr<Peer> peer);
            virtual void onIncomingConnect(std::shared_ptr<Peer> peer);
            virtual void onDisconnect(std::shared_ptr<Peer> peer);
            virtual void onMessage(std::shared_ptr<Peer> peer, Message& msg);

            // Thread safe queue for incoming messages
            tsqueue<OwnedMessage> _messagesIn;

            // Container for active peers
            std::deque<std::shared_ptr<Peer>> _peers{};

            // Container for info of potential connections
            std::deque<NodeInfo> _potentialConnections{};
            
            // Thread for handling incoming messages
            std::thread _responseThread;

            // Info of active nodes on network, info of connected nodes, self info
            std::set<NodeInfo> _activeNodeList;
            std::set<NodeInfo> _connectedNodeList;
            NodeInfo           _selfInfo;
            bool               _recievedNodeList = false;

            // Asio context as well as its own thread to run in
            boost::asio::io_context _context;
            std::thread             _contextThread;

            std::condition_variable _connectionBlocking;
            std::mutex              _connectionMutex;

            // Thread for managing connections
            std::thread _peerManagementThread;

            // Asio acceptor
            boost::asio::ip::tcp::acceptor _acceptor;

            // Connections will be identified in the system by an id
            uint16_t _idCounter = 10000;
    };
}

#endif