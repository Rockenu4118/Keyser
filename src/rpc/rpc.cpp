#include "./rpc.hpp"
#include "../node/Node.hpp"
#include <nlohmann/json.hpp>

keyser::RPC::RPC(Node *node) : _acceptor(_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 8080))
{
    _node = node;
}

keyser::RPC::~RPC() { shutdown(); }

void keyser::RPC::run()
{
    boost::asio::io_context::work idlework(_context);

    _contextThr = std::thread([this]()
                              { _context.run(); });

    _rpcThr = std::thread([this]()
                          { startServer(); });
}

void keyser::RPC::startServer()
{
    while (true)
    {
        boost::asio::ip::tcp::socket socket(_context);
        _acceptor.accept(socket);

        // Read the HTTP request
        boost::beast::flat_buffer buffer;
        boost::beast::http::request<boost::beast::http::string_body> request;
        boost::beast::http::read(socket, buffer, request);

        // Handle the request
        onMessage(request, socket);

        // Close the socket
        socket.shutdown(boost::asio::ip::tcp::socket::shutdown_send);
    }
}

void keyser::RPC::shutdown()
{}

void keyser::RPC::onMessage(const boost::beast::http::request<boost::beast::http::string_body> &request, boost::asio::ip::tcp::socket &socket)
{
    // Prepare response
    boost::beast::http::response<boost::beast::http::string_body> response;
    response.version(request.version());
    response.set(boost::beast::http::field::server, "My HTTP Server");
    response.set(boost::beast::http::field::content_type, "text/plain");

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
    if      (method == "getBalance")  handleGetBalance(response, params);
    else if (method == "getHeight")   handleGetHeight(response, params);
    else if (method == "getBlock")    handleGetBlock(response, params);
    else if (method == "getPeerInfo") handleGetPeerInfo(response, params);
    else if (method == "shutdown")    handleShutdown(response, params);
    else                              handleInvalidMethod(response);


    // Send the response to the client
    response.prepare_payload();
    boost::beast::http::write(socket, response);
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

void keyser::RPC::handleGetBlock(boost::beast::http::response<boost::beast::http::string_body>& response, std::vector<std::string> &params)
{
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
    
    nlohmann::json res;

    res["index"]         = block->_index;
    res["time"]          = block->_time;
    res["nonce"]         = block->_nonce;
    res["prevHash"]      = block->_prevHash;
    res["hash"]          = block->_hash;
    res["reward"]        = block->_reward;
    res["rewardAddress"] = block->_rewardAddress;
 
    response.result(boost::beast::http::status::ok);
    response.body() = res.dump();
}

void keyser::RPC::handleGetPeerInfo(boost::beast::http::response<boost::beast::http::string_body>& response, std::vector<std::string>& params)
{
    nlohmann::json res;

    for (auto& info : _node->getConnections())
    {
        nlohmann::json peerInfo;
        peerInfo["version"] = info._version;
        peerInfo["alias"]   = info._alias;
        peerInfo["address"] = info._address;
        peerInfo["port"]    = info._port;

        res["peerInfo"].push_back(peerInfo);
    }

    response.result(boost::beast::http::status::ok);
    response.body() = res.dump();
}

void keyser::RPC::handleShutdown(boost::beast::http::response<boost::beast::http::string_body>& response, std::vector<std::string>& params)
{}

void keyser::RPC::handleInvalidMethod(boost::beast::http::response<boost::beast::http::string_body>& response)
{
    response.result(boost::beast::http::status::bad_request);
    response.body() = "Invalid method.";
}