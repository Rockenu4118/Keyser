#ifndef NET_INTERFACE_H
#define NET_INTERFACE_H

#include <vector>
#include <string>
#include <boost/asio.hpp>
#include <unordered_map>
#include <tsqueue.hpp>

#include "./Client.hpp"
#include "./Server.hpp"
#include "./Peer.hpp"
#include "../net/Message.hpp"
#include "../node/PeerInfo.hpp"
#include "../chain/Chain.hpp"
#include "../chain/Mempool.hpp"
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

            void startServer();

            void message(std::shared_ptr<Peer> peer, const Message& msg);
            void messageNeighbors(const Message& msg, std::shared_ptr<Peer> ignorePeer = nullptr);
            void managePeers();

            void ping();
            void pong();

            // Handle incoming messages
            void update(uint8_t maxMessages = -1, bool wait = true);

            // Accessors
            int connectionCount() const;

            PeerInfo& selfInfo();
            
            std::shared_ptr<Peer> syncNode();

            uint16_t assignId();

            void displayPeers();
            void displayListeningNodes();
            void displaySelfInfo();

            std::vector<PeerInfo> getConnections() const;

            bool validateConnection(std::shared_ptr<Peer>& connection);

            void inv(std::shared_ptr<Peer> peer);

            void distributePeerInfo(PeerInfo& peerInfo);
            void distributeBlock(Block& block);
            void distributeTransaction(Transaction& transaction);

            void onDisconnect(std::shared_ptr<Peer> peer);
            void onMessage(std::shared_ptr<Peer> peer, Message& msg);

            void handlePing(std::shared_ptr<Peer> peer, Message& msg);
            void handleDistributePeerInfo(std::shared_ptr<Peer> peer, Message& msg);
            void handleDistributeBlock(std::shared_ptr<Peer> peer, Message& msg);
            void handleDistributeTransaction(std::shared_ptr<Peer> peer, Message& msg);

            std::shared_ptr<Client>& client();
            std::shared_ptr<Server>& server();

            std::deque<std::shared_ptr<Peer>>& peers();
            
            std::unordered_map<std::string, NodeInfo>& listeningNodes();

            tsqueue<OwnedMessage>& messagesIn();

        private:
            Node* _node;

            std::shared_ptr<Client> _client;
            std::shared_ptr<Server> _server;
            
            // Thread safe queue for incoming messages
            tsqueue<OwnedMessage> _messagesIn;

            // Container for active peers
            std::deque<std::shared_ptr<Peer>> _peers;

            // Container for listening nodes
            std::unordered_map<std::string, NodeInfo> _listeningNodes;

            // Asio context and thread to run in
            boost::asio::io_context _context;
            std::thread _contextThr;
            
            // Thread for handling incoming messages
            std::thread _responseThread;

            // Info of active nodes on network, info of connected nodes, self info
            PeerInfo _selfInfo;

            // Connections will be identified in the system by an id
            uint16_t _idCounter = 10000;
    };
}

#endif