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
            NetInterface(uint16_t port, bool& shutdownNode);
            
            virtual ~NetInterface();

            bool startServer();
            void shutdown();
            bool connect(const NodeInfo nodeInfo);
            void acceptConnection();
            void message(std::shared_ptr<Peer> peer, const Message& msg);
            void messageNeighbors(const Message& msg, std::shared_ptr<Peer> ignorePeer = nullptr);
            void managePeers();

            // Handle incoming messages
            void update(uint8_t maxMessages = -1, bool wait = true);

            // Accessors
            int connectionCount() const;
            NodeInfo getSelfInfo() const;
            
            std::shared_ptr<Peer> syncNode();

            void displayConnections();
            void displayActiveNodes();
            void displaySelfInfo();

            std::vector<NodeInfo> getConnections() const;
            std::set<NodeInfo>    getActiveNodes() const;

            bool validateConnection(std::shared_ptr<Peer>& connection);

        protected:
            virtual bool allowConnect(std::shared_ptr<Peer> peer);
            virtual void onOutgoingConnect(std::shared_ptr<Peer> peer);
            virtual void onIncomingConnect(std::shared_ptr<Peer> peer);
            virtual void onDisconnect(std::shared_ptr<Peer> peer);
            virtual void onMessage(std::shared_ptr<Peer> peer, Message& msg);

            // Asio context as well as its own thread to run in
            boost::asio::io_context _context;
            std::thread             _contextThread;

            // Thread safe queue for incoming messages
            tsqueue<OwnedMessage> _messagesIn;

            // Container for info of potential connections
            std::deque<NodeInfo> _potentialConnections;

            // Container for active peers
            std::deque<std::shared_ptr<Peer>> _peers;
            std::set<NodeInfo>                _connectedNodeList;
            std::set<NodeInfo>                _activeNodeList;
            
            // Thread for handling incoming messages
            std::thread _responseThread;

            // Info of active nodes on network, info of connected nodes, self info
            NodeInfo _selfInfo;
            bool     _recievedNodeList = false;

            // Asio acceptor
            boost::asio::ip::tcp::acceptor _acceptor;

            // Connections will be identified in the system by an id
            uint16_t _idCounter = 10000;

            bool& _shutdownNode;
    };
}

#endif