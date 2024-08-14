#include "./Server.hpp"


keyser::Server::Server(Node* node, NetInterface* network, uint16_t port, boost::asio::io_context& context, std::thread& contextThr) 
    : _node(node), _network(network), _acceptor(_serverContext, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)), _context(context), _contextThr(contextThr)
{}

keyser::Server::~Server()
{
    stop();
}

bool keyser::Server::start()
{
    try
    {
        // Setup server to be able to accept connections
        // boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), _network->getSelfInfo()._port);
        // _acceptor.open(endpoint.protocol());
        // _acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
        // _acceptor.bind(endpoint);
        // _acceptor.listen();

        acceptConnection();

        _serverContextThr = std::thread([this]() { _serverContext.run(); });
    }
    catch (const std::exception& e)
    {
        std::cerr << "[SERVER] Exception: " << e.what() << std::endl;
        return false; 
    }

    announceSelf();

    std::cout << "[SERVER] Started!" << std::endl;
    return false;
}

void keyser::Server::stop()
{
    _acceptor.close();

    _serverContext.stop();

    if (_serverContextThr.joinable())
        _serverContextThr.join();
}

void keyser::Server::acceptConnection()
{
    _acceptor.async_accept(
        [this](std::error_code ec, boost::asio::ip::tcp::socket socket)
        {
            if (!ec)
            {
                std::cout << "[SERVER] New connection: " << socket.remote_endpoint() << std::endl;

                auto newConn = std::make_shared<Peer>(PeerInfo::Direction::Inbound, _serverContext, std::move(socket), _network->messagesIn(), _network->assignId());

                newConn->info().endpoint.address = newConn->getEndpoint().address().to_string();

                // Give node a chance to deny connection
                if (allowConnect(newConn))
                {
                    onIncomingConnect(newConn);

                    // Listen for message
                    newConn->listen();
                    
                    // Connection allowed, connection pushed to back of connection container
                    _network->peers().push_back(std::move(newConn));

                    std::cout << "[" << _network->peers().back()->getId() << "] Connection Approved" << std::endl;
                }
                else
                {
                    std::cout << "[SERVER] Connection Denied" << std::endl;
                }
            }
            else
            {
                // Error during acceptance
                std::cout << "[SERVER] New connection error: " << ec.message() << std::endl;
            }

            // Prime asio context with more work, wait for another connection...
            acceptConnection();
        }
    );
}

void keyser::Server::announceSelf()
{
    _network->listeningNodes().insert(std::pair(_network->selfInfo().endpoint.string(), _network->selfInfo()));

    Message msg(MsgTypes::DistributeNodeInfo);
    msg.json() = _network->selfInfo().json();
    msg.preparePayload();
    _network->messageNeighbors(msg);
}

void keyser::Server::verack(std::shared_ptr<Peer> peer)
{
    Message newMsg(MsgTypes::Verack);
    newMsg.json()["Outbound version"] = _network->selfInfo().version;
    newMsg.json()["Outbound port"]    = _network->selfInfo().endpoint.port;
    newMsg.json()["address"]          = peer->getEndpoint().address().to_string();

    newMsg.preparePayload();
    _network->message(peer, newMsg);
}

void keyser::Server::nodeInfoStream(std::shared_ptr<Peer> peer)
{
    Message msg(MsgTypes::NodeInfoList);

    msg.json() = nlohmann::json::array();

    for (const auto&[key, info] : _network->listeningNodes())
        msg.json().push_back(info.json());

    msg.preparePayload();
    _network->message(peer, msg);
}

void keyser::Server::sendHeaders(std::shared_ptr<Peer> peer, std::string latestHash)
{
    Message msg(MsgTypes::Headers);
    msg.json() = nlohmann::json::array();

    int index = _node->chain()->headers().at(latestHash)._index;
    auto iter = std::next(_node->chain()->blockIndex().find(index));

    if (iter == _node->chain()->blockIndex().end())
        return;

    for ( ; iter != _node->chain()->blockIndex().end() ; iter++)
    {
        BlockHeader header = _node->chain()->headers().at(iter->second);
        msg.json().push_back(header.json());
    }

    msg.preparePayload();
    _network->message(peer, msg);
}

void keyser::Server::sendBlock(std::shared_ptr<Peer> peer, std::string hash)
{
    Message msg(MsgTypes::Block);
    msg.json() = _node->chain()->blocks().at(hash).json();

    msg.preparePayload();
    _network->message(peer, msg);
}

void keyser::Server::handleVersion(std::shared_ptr<Peer> peer, Message& msg)
{
    msg.unpackPayload();

    // Save external address
    _network->selfInfo().endpoint.address = msg.json()["address"];

    // unpackPayload incoming node info
    peer->info().version       = msg.json()["Outbound version"];
    peer->info().endpoint.port = msg.json()["Outbound port"];

    // Send self info as well as their external ip
    verack(peer);
}

void keyser::Server::handleGetNodeList(std::shared_ptr<Peer> peer, Message& msg)
{
    nodeInfoStream(peer);
}

void keyser::Server::handleGetHeaders(std::shared_ptr<Peer> peer, Message& msg)
{
    msg.unpackPayload();

    sendHeaders(peer, msg.json()["latestHash"]);
}

void keyser::Server::handleGetData(std::shared_ptr<Peer> peer, Message& msg)
{
    msg.unpackPayload();

    if (msg.json()["type"] == DataTypes::Block)
    {
        Message newMsg(MsgTypes::Block);
        newMsg.json() = _node->chain()->blocks().at(msg.json()["hash"]).json();

        newMsg.preparePayload();
        _network->message(peer, newMsg);
    }

    // if (msg.json()["type"] == DataTypes::TX)
    // {

    // }
}

bool keyser::Server::allowConnect(std::shared_ptr<Peer> peer)
{ return true; }

void keyser::Server::onIncomingConnect(std::shared_ptr<Peer> peer)
{}