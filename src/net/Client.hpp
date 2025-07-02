#ifndef CLIENT_H
#define CLIENT_H

#include <memory>
#include <boost/asio.hpp>

#include "../node/Node.hpp"
#include "./Peer.hpp"


enum class DataTypes : uint8_t;

namespace keyser
{
    class Node;
    class NetInterface;

    class Client
    {
        public:
            Client(Node* node, NetInterface* network, boost::asio::io_context& context, std::thread& contextThr);

            ~Client();

            bool connect(const Endpoint endpoint);

            bool allowConnect(Endpoint endpoint) const;

            void version(std::shared_ptr<Peer> peer);

            void getNodeList();

            void getHeaders();

            void getData(DataTypes type, std::string hash);

            void completedInitialBlockDownload();

            // Msg Handlers
            void handleVerack(std::shared_ptr<Peer> peer, NetMessage& msg);
            void handlePeerInfo(std::shared_ptr<Peer> peer, NetMessage& msg);
            void handleHeaders(std::shared_ptr<Peer> peer, NetMessage& msg);
            void handleInv(std::shared_ptr<Peer> peer, NetMessage& msg);
            void handleBlock(std::shared_ptr<Peer> peer, NetMessage& msg);

        private:
            void onOutgoingConnect(std::shared_ptr<Peer> peer);

            Node* _node;
            NetInterface* _network;

            boost::asio::io_context& _context;

            std::thread& _contextThr;

            boost::asio::io_context _clientContext;

            std::thread _clientContextThr;

    };
}


#endif