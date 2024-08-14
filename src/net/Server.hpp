#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <memory>
#include <boost/asio.hpp>

#include "../node/Node.hpp"
#include "./NetInterface.hpp"
#include "../node/PeerInfo.hpp"
#include "../net/Peer.hpp"

namespace keyser
{
    class Node;
    class NetInterface;

    class Server
    {
        public:
            Server(Node* node, NetInterface* network, uint16_t port, boost::asio::io_context& context, std::thread& contextThr);

            ~Server();

            bool start();

            void stop();

            void acceptConnection();

            void announceSelf();

            void verack(std::shared_ptr<Peer> peer);

            void nodeInfoStream(std::shared_ptr<Peer> peer);

            void sendHeaders(std::shared_ptr<Peer> peer, std::string latestHash);

            void sendBlock(std::shared_ptr<Peer> peer, std::string hash);

            // Msg Handlers
            void handleVersion(std::shared_ptr<Peer> peer, Message& msg);
            void handleGetNodeList(std::shared_ptr<Peer> peer, Message& msg);
            void handleGetHeaders(std::shared_ptr<Peer> peer, Message& msg);
            void handleGetData(std::shared_ptr<Peer> peer, Message& msg);
        
        private:
            bool allowConnect(std::shared_ptr<Peer> peer);
            void onIncomingConnect(std::shared_ptr<Peer> peer);

            Node* _node;

            NetInterface* _network;

            // Asio context
            boost::asio::io_context& _context;
            std::thread& _contextThr;

            boost::asio::io_context _serverContext;
            std::thread _serverContextThr;

            // Asio acceptor
            boost::asio::ip::tcp::acceptor _acceptor; 
    };
}

#endif