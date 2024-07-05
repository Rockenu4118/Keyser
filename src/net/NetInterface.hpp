#ifndef NET_INTERFACE_H
#define NET_INTERFACE_H

#include <set>
#include <vector>
#include <string>
#include <mutex>
#include <condition_variable>
#include <boost/asio.hpp>
#include <tsqueue.hpp>

#include "./Client.hpp"
#include "./Server.hpp"
#include "./Peer.hpp"
#include "../net/Message.hpp"
#include "../node/NodeInfo.hpp"
#include "../chain/Chain.hpp"
#include "../chain/Mempool.hpp"
#include "../storage/StorageEngine.hpp"
#include "../utils/utils.hpp"


namespace keyser
{
    class Client;
    class Server;
    class Peer;

    class NetInterface
    {
        public:
            NetInterface(Node* node, uint16_t port);
            
            ~NetInterface();

            void message(std::shared_ptr<Peer> peer, const Message& msg);
            void messageNeighbors(const Message& msg, std::shared_ptr<Peer> ignorePeer = nullptr);
            void managePeers();

            void ping();
            void pong();

            // Handle incoming messages
            void update(uint8_t maxMessages = -1, bool wait = true);

            // Accessors
            int connectionCount() const;

            NodeInfo getSelfInfo() const;
            
            std::shared_ptr<Peer> syncNode();

            uint16_t assignId();

            void displayConnections();
            void displayActiveNodes();
            void displaySelfInfo();

            std::vector<NodeInfo> getConnections() const;
            std::set<NodeInfo>    getActiveNodes() const;

            bool validateConnection(std::shared_ptr<Peer>& connection);

            void distributeNodeInfo(NodeInfo& nodeInfo);
            void distributeBlock(Block& block);
            void distributeTransaction(Transaction& transaction);

            void onDisconnect(std::shared_ptr<Peer> peer);
            void onMessage(std::shared_ptr<Peer> peer, Message& msg);

            void handlePing(std::shared_ptr<Peer> peer, Message& msg);
            void handleDistributeNodeInfo(std::shared_ptr<Peer> peer, Message& msg);
            void handleDistributeBlock(std::shared_ptr<Peer> peer, Message& msg);
            void handleDistributeTransaction(std::shared_ptr<Peer> peer, Message& msg);

            std::shared_ptr<Client>& client();
            std::shared_ptr<Server>& server();

            std::deque<std::shared_ptr<Peer>>& peers();
            
            std::set<NodeInfo>&    connectedNodeList();
            std::set<NodeInfo>&    activeNodeList();
            std::deque<NodeInfo>&  potentialConnections();
            tsqueue<OwnedMessage>& messagesIn();

        private:
            Node* _node;

            std::shared_ptr<Client> _client;
            std::shared_ptr<Server> _server;
            
            // Thread safe queue for incoming messages
            tsqueue<OwnedMessage> _messagesIn;

            // Asio context as well as its own thread to run in
            boost::asio::io_context _context;
            std::thread             _contextThread;

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

            // Connections will be identified in the system by an id
            uint16_t _idCounter = 10000;
    };
}

#endif