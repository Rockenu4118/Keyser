#ifndef RPC_H
#define RPC_H

#include <string>
#include <vector>
#include <thread>
#include <tsqueue.hpp>

#include <boost/asio.hpp>
#include <boost/beast.hpp>

#include "../node/Node.hpp"


namespace keyser
{
    class RPC
    {
        public:
            RPC(Node* node);

            ~RPC();

            void run();

            void startServer();

            void shutdown();

            void onMessage(const boost::beast::http::request<boost::beast::http::string_body>& request, boost::asio::ip::tcp::socket& socket);

            void handleGetBalance(boost::beast::http::response<boost::beast::http::string_body>& response, const std::vector<std::string>& params);

            void handleGetHeight(boost::beast::http::response<boost::beast::http::string_body>& response, const std::vector<std::string>& params);

            void handleGetBlock(boost::beast::http::response<boost::beast::http::string_body>& response, std::vector<std::string>& params);

            void handleGetPeerInfo(boost::beast::http::response<boost::beast::http::string_body>& response, std::vector<std::string>& params);

            void handleShutdown(boost::beast::http::response<boost::beast::http::string_body>& response, std::vector<std::string>& params);

            void handleInvalidMethod(boost::beast::http::response<boost::beast::http::string_body>& response);

        private:
            Node* _node;

            std::thread _rpcThr;

            std::thread _contextThr;

            boost::asio::io_context _context;

            boost::asio::ip::tcp::acceptor _acceptor;
    };
}

#endif