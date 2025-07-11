#ifndef NET_INTERFACE_H
#define NET_INTERFACE_H

#include <vector>
#include <string>
#include <boost/asio.hpp>
#include <unordered_map>
#include "../utils/tsqueue.hpp"

#include "./Peer.hpp"
#include "../net/NetMessage.hpp"
#include "../node/PeerInfo.hpp"
#include "../chain/ChainIndex.hpp"
#include "../chain/Mempool.hpp"
#include "../utils/utils.hpp"
#include "../views/MainMenu.hpp"


namespace keyser
{
    class MainMenu;
    class Peer;

    class NetInterface
    {
    public:
        NetInterface(Node* node, MainMenu* menu, uint16_t port);
            
        ~NetInterface();

        bool connect(const Endpoint endpoint);

        bool allowConnect(Endpoint endpoint) const;

        bool start();

        void stop();

        void acceptConnection();

        void message(std::shared_ptr<Peer> peer, const NetMessage& msg);
        void messageNeighbors(const NetMessage& msg, std::shared_ptr<Peer> ignorePeer = nullptr);
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

            std::vector<PeerInfo> getConnections() const;

            bool validateConnection(std::shared_ptr<Peer>& connection);

            void inv(std::shared_ptr<Peer> peer);

            void distributePeerInfo(PeerInfo& peerInfo);
            void distributeBlock(Block& block);
            void distributeTransaction(Transaction& transaction);

            void onDisconnect(std::shared_ptr<Peer> peer);
            void onMessage(std::shared_ptr<Peer> peer, NetMessage& msg);

            void handlePing(std::shared_ptr<Peer> peer, NetMessage& msg);
            void handleDistributePeerInfo(std::shared_ptr<Peer> peer, NetMessage& msg);
            void handleDistributeBlock(std::shared_ptr<Peer> peer, NetMessage& msg);
            void handleDistributeTransaction(std::shared_ptr<Peer> peer, NetMessage& msg);

        void onOutgoingConnect(std::shared_ptr<Peer> peer);
        bool allowConnect(std::shared_ptr<Peer> peer);
        void onIncomingConnect(std::shared_ptr<Peer> peer);



        // Thread safe queue for incoming messages
        tsqueue<OwnedMessage> mMessagesIn;

        // Container for active peers
        std::deque<std::shared_ptr<Peer>> mPeers;


        // Container for listening nodes
        std::unordered_map<std::string, NodeInfo> _listeningNodes;

        // Asio context and thread to run in
        boost::asio::io_context _context;
        std::thread _contextThr;

        // Asio acceptor
        boost::asio::ip::tcp::acceptor _acceptor;
            
        // Thread for handling incoming messages
        std::thread _responseThread;

        // Info of active nodes on network, info of connected nodes, self info
        PeerInfo _selfInfo;

        // Connections will be identified in the system by an id
        uint16_t _idCounter = 10000;

    private:
        Node* mNode;
        MainMenu* mMenu;
    };
}

#endif