#ifndef RPC_H
#define RPC_H

#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <tsqueue.hpp>

#include <boost/asio.hpp>
#include <boost/beast.hpp>

#include "../node/Node.hpp"


namespace keyser
{
    class RPC
    {
        public:
            RPC(Node* node, uint16_t port);

            ~RPC();

            void run();

            void acceptRequest();

            void shutdown();

            void onMessage(const boost::beast::http::request<boost::beast::http::string_body>& request, boost::asio::ip::tcp::socket& socket);

            void handlePing(boost::beast::http::response<boost::beast::http::string_body>& response, const std::vector<std::string>& params);

            void handleGetWallets(boost::beast::http::response<boost::beast::http::string_body>& response, const std::vector<std::string>& params);

            void handleCreateWallet(boost::beast::http::response<boost::beast::http::string_body>& response, const std::vector<std::string>& params);

            void handleGetBalance(boost::beast::http::response<boost::beast::http::string_body>& response, const std::vector<std::string>& params);

            void handleGetHeight(boost::beast::http::response<boost::beast::http::string_body>& response, const std::vector<std::string>& params);

            void handleGetBlock(boost::beast::http::response<boost::beast::http::string_body>& response, const std::vector<std::string>& params);

            void handleGetBlocks(boost::beast::http::response<boost::beast::http::string_body>& response, const std::vector<std::string>& params);

            void handleGetMempool(boost::beast::http::response<boost::beast::http::string_body>& response, const std::vector<std::string>& params);

            void handleBeginMining(boost::beast::http::response<boost::beast::http::string_body>& response, const std::vector<std::string>& params);

            void handleStopMining(boost::beast::http::response<boost::beast::http::string_body>& response, const std::vector<std::string>& params);

            void handleGetSelfInfo(boost::beast::http::response<boost::beast::http::string_body>& response, const std::vector<std::string>& params);

            void handleGetPeerInfo(boost::beast::http::response<boost::beast::http::string_body>& response, const std::vector<std::string>& params);

            void handleGetActiveNodeInfo(boost::beast::http::response<boost::beast::http::string_body>& response, const std::vector<std::string>& params);

            void handleGetUptime(boost::beast::http::response<boost::beast::http::string_body>& response, const std::vector<std::string>& params);

            void handleShutdown(boost::beast::http::response<boost::beast::http::string_body>& response, const std::vector<std::string>& params);

            void handleInvalidMethod(boost::beast::http::response<boost::beast::http::string_body>& response);

        private:
            Node* _node;

            boost::asio::io_context _context;

            std::thread _contextThr;

            boost::asio::ip::tcp::acceptor _acceptor;
    };
}

#endif