#ifndef CLIENT_H
#define CLIENT_H

#include <memory>

#include <boost/asio.hpp>

#include "../node/Node.hpp"
#include "./Peer.hpp"


namespace keyser
{
    class Node;
    class NetInterface;

    class Client
    {
        public:
            Client(Node* node, NetInterface* network, boost::asio::io_context& context);

            ~Client() = default;

            bool connect(const NodeInfo nodeInfo);

            void version(std::shared_ptr<Peer> peer);

            void getNodeList();

            void getHeaders();

            void getBlocks();

            void getData();

            void completedInitialBlockDownload();

            // Msg Handlers
            void handleVerack(std::shared_ptr<Peer> peer, Message& msg);
            void handleNodeInfo(std::shared_ptr<Peer> peer, Message& msg);
            void handleHeaders(std::shared_ptr<Peer> peer, Message& msg);
            void handleInv(std::shared_ptr<Peer> peer, Message& msg);
            void handleBlock(std::shared_ptr<Peer> peer, Message& msg);

        private:
            void onOutgoingConnect(std::shared_ptr<Peer> peer);

            Node* _node;
            NetInterface* _network;

            boost::asio::io_context& _context;

    };
}


#endif