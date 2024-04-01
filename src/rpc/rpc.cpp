#include <nlohmann/json.hpp>

#include "./rpc.hpp"
#include "../node/Node.hpp"
#include "../utils/utils.hpp"


keyser::RPC::RPC(Node *node, uint16_t port) : _acceptor(_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
{
    _node = node;
}

keyser::RPC::~RPC() { shutdown(); }

void keyser::RPC::run()
{
    acceptRequest();

    _contextThr = std::thread([this]() { _context.run(); });    
}

void keyser::RPC::acceptRequest()
{
    try
    {
        _acceptor.async_accept(
            [this](std::error_code ec, boost::asio::ip::tcp::socket socket)
            {
                if (!ec)
                {
                    std::cout << "[RPC] New Req: " << socket.remote_endpoint() << std::endl;

                    boost::beast::flat_buffer buffer;
                    boost::beast::http::request<boost::beast::http::string_body> request;

                    try
                    {
                        boost::beast::http::read(socket, buffer, request);
                    }
                    catch(const boost::wrapexcept<boost::system::system_error>& e)
                    {
                        std::cerr << "[RPC] Exception: " << e.what() << std::endl;
                    }
                    
                    // Handle the request
                    onMessage(request, socket);

                    // Close the socket
                    socket.shutdown(boost::asio::ip::tcp::socket::shutdown_send);
                }
                else
                {
                    // Error during request
                    std::cout << "[RPC] Connection Error: " << ec.message() << std::endl; 
                }
                
                // Prime asio context with more work, wait for another request
                acceptRequest();
            }
        );
    }
    catch(const std::exception& e)
    {
        std::cerr << "[RPC] Exception: " << e.what() << std::endl;
    }
    
}

void keyser::RPC::shutdown()
{
    // Request the context to close
    _context.stop();

    // Join context thread
    if (_contextThr.joinable())
        _contextThr.join();

    std::cout << "[RPC] Stopped!" << std::endl;
}

void keyser::RPC::onMessage(const boost::beast::http::request<boost::beast::http::string_body> &request, boost::asio::ip::tcp::socket &socket)
{
    // Prepare response
    boost::beast::http::response<boost::beast::http::string_body> response;
    response.version(request.version());
    response.set(boost::beast::http::field::server, "My HTTP Server");
    response.set(boost::beast::http::field::content_type, "text/plain");
    response.set(boost::beast::http::field::access_control_allow_origin, "*");

    // Parse request
    std::string method;
    std::vector<std::string> params;

    try
    {
        nlohmann::json req;
        req = nlohmann::json::parse(request.body());

        method = req["method"];
        params = req["params"];
    }
    catch (std::exception &err)
    {
        std::cerr << err.what() << std::endl;
        
        // Respond with an error msg
        response.result(boost::beast::http::status::bad_request);
        response.body() = "Invalid request.";
        response.prepare_payload();
        boost::beast::http::write(socket, response);
        return;
    }

    // Handle each method differently
    try
    {
        if      (method == "ping")         handlePing         (response, params);
        else if (method == "getWallets")   handleGetWallets   (response, params);
        else if (method == "createWallet") handleCreateWallet (response, params);
        else if (method == "getBalance")   handleGetBalance   (response, params);
        else if (method == "getHeight")    handleGetHeight    (response, params);
        else if (method == "getBlock")     handleGetBlock     (response, params);
        else if (method == "getBlocks")    handleGetBlocks    (response, params);
        else if (method == "getMempool")   handleGetMempool   (response, params);
        else if (method == "getPeerInfo")  handleGetPeerInfo  (response, params);
        else if (method == "getUptime")    handleGetUptime    (response, params);
        else if (method == "shutdown")     handleShutdown     (response, params);
        else                               handleInvalidMethod(response);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    // Send the response to the client
    response.prepare_payload();
    boost::beast::http::write(socket, response);
}

void keyser::RPC::handlePing(boost::beast::http::response<boost::beast::http::string_body>& response, const std::vector<std::string>& params)
{
    nlohmann::json res;

    response.result(boost::beast::http::status::ok);
    response.body() = res.dump();
}

void keyser::RPC::handleGetWallets(boost::beast::http::response<boost::beast::http::string_body>& response, const std::vector<std::string>& params)
{
    nlohmann::json res = nlohmann::json::array();

    for (int i = 0 ; i < _node->walletManager().count() ; i++)
    {
        nlohmann::json wallet;
        wallet["name"] = _node->walletManager().at(i).getName();
        wallet["publicAddress"] = _node->walletManager().at(i).getPublicAddress();

        res.push_back(wallet);
    }

    response.result(boost::beast::http::status::ok);
    response.body() = res.dump(2);
}

void keyser::RPC::handleCreateWallet(boost::beast::http::response<boost::beast::http::string_body>& response, const std::vector<std::string>& params)
{
    nlohmann::json res;

    _node->walletManager().createWallet(params.at(0));

    response.result(boost::beast::http::status::ok);
    response.body() = res.dump();
}

void keyser::RPC::handleGetBalance(boost::beast::http::response<boost::beast::http::string_body>& response, const std::vector<std::string> &params)
{
    nlohmann::json res;
    res["balance"] = _node->getAddressBalance(params.at(0));

    response.result(boost::beast::http::status::ok);
    response.body() = res.dump();
}

void keyser::RPC::handleGetHeight(boost::beast::http::response<boost::beast::http::string_body>& response, const std::vector<std::string> &params)
{
    nlohmann::json res;
    res["height"] = _node->getHeight();

    response.result(boost::beast::http::status::ok);
    response.body() = res.dump();
}

void keyser::RPC::handleGetBlock(boost::beast::http::response<boost::beast::http::string_body>& response, const std::vector<std::string> &params)
{
    nlohmann::json res;

    std::shared_ptr<Block> block;

    try
    {
        block = _node->getBlock(stoi(params.at(0)));
    }
    catch(const std::out_of_range& err)
    {
        std::cerr << err.what() << std::endl;

        response.result(boost::beast::http::status::bad_request);
        response.body() = "Invalid block.";
        return;
    }
    
    utils::encodeJson(res, *block);
 
    response.result(boost::beast::http::status::ok);
    response.body() = res.dump();
}

void keyser::RPC::handleGetBlocks(boost::beast::http::response<boost::beast::http::string_body>& response, const std::vector<std::string>& params)
{
    nlohmann::json res = nlohmann::json::array();

    int first = stoi(params.at(0));
    int last  = stoi(params.at(1));

    if ((last - first) > 100)
    {
        response.result(boost::beast::http::status::bad_request);
        response.body() = "Excessive blocks requested.";
        return;
    }

    std::shared_ptr<Block> block;

    try
    {
        for (int i = first ; i <= last ; i++)
        {
            nlohmann::json jsonBlock;
            block = _node->getBlock(i);
            utils::encodeJson(jsonBlock, *block);
            res.push_back(jsonBlock);
        }  
    }
    catch(const std::out_of_range& e)
    {
        std::cerr << e.what() << '\n';

        response.result(boost::beast::http::status::bad_request);
        response.body() = "Invalid blocks.";
        return;
    }
    
    response.result(boost::beast::http::status::ok);
    response.body() = res.dump();
}

void keyser::RPC::handleGetMempool(boost::beast::http::response<boost::beast::http::string_body>& response, const std::vector<std::string>& params)
{
    nlohmann::json res = nlohmann::json::array();

    for (auto& tx : _node->pendingTransactions())
    {
        nlohmann::json transaction;
        utils::encodeJson(transaction, tx);
        res.push_back(transaction);
    }

    response.result(boost::beast::http::status::ok);
    response.body() = res.dump();
}

void keyser::RPC::handleGetPeerInfo(boost::beast::http::response<boost::beast::http::string_body>& response, const std::vector<std::string>& params)
{
    nlohmann::json res = nlohmann::json::array();

    for (auto& info : _node->getConnections())
    {
        nlohmann::json peerInfo;
        peerInfo["version"] = info._version;
        peerInfo["alias"]   = info._alias;
        peerInfo["address"] = info._address;
        peerInfo["port"]    = info._port;

        res.push_back(peerInfo);
    }

    response.result(boost::beast::http::status::ok);
    response.body() = res.dump();
}

void keyser::RPC::handleGetUptime(boost::beast::http::response<boost::beast::http::string_body>& response, const std::vector<std::string>& params)
{
    nlohmann::json res;

    res["uptime"] = _node->getUptime();

    response.result(boost::beast::http::status::ok);
    response.body() = res.dump();
}

void keyser::RPC::handleShutdown(boost::beast::http::response<boost::beast::http::string_body>& response, const std::vector<std::string>& params)
{
    std::cout << "Handle shutdown" << std::endl;
}

void keyser::RPC::handleInvalidMethod(boost::beast::http::response<boost::beast::http::string_body>& response)
{
    response.result(boost::beast::http::status::bad_request);
    response.body() = "Invalid method.";
}