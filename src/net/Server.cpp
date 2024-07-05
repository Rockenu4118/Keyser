#include "./Server.hpp"


keyser::Server::Server(Node* node, NetInterface* network, uint16_t port, boost::asio::io_context& context) 
    : _node(node), _network(network), _acceptor(context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)), _context(context)
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
        boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), _network->getSelfInfo()._port);
        // _acceptor.open(endpoint.protocol());
        // _acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
        // _acceptor.bind(endpoint);
        // _acceptor.listen();

        acceptConnection();
    }
    catch (std::exception& e)
    {
        std::cerr << "[NODE] Exception: " << e.what() << std::endl;
        return false; 
    }

    std::cout << "[NODE] Started!" << std::endl;
    return false;
}

void keyser::Server::stop()
{
    _acceptor.close();
}

void keyser::Server::acceptConnection()
{
    _acceptor.async_accept(
        [this](std::error_code ec, boost::asio::ip::tcp::socket socket)
        {
            if (!ec)
            {
                std::cout << "[NODE] New connection: " << socket.remote_endpoint() << std::endl;

                auto newConn = std::make_shared<Peer>(NodeInfo::Direction::Inbound, _context, std::move(socket), _network->messagesIn(), _network->assignId());

                newConn->info()._address = newConn->getEndpoint().address().to_string();
                newConn->info()._port = newConn->getEndpoint().port();

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
                    std::cout << "[NODE] Connection Denied" << std::endl;
                }
            }
            else
            {
                // Error during acceptance
                std::cout << "[NODE] New connection error: " << ec.message() << std::endl;
            }

            // Prime asio context with more work, wait for another connection...
            acceptConnection();
        }
    );
}

void keyser::Server::broadcastSelf()
{
    _network->activeNodeList().insert(_network->getSelfInfo());

    Message msg(MsgTypes::DistributeNodeInfo);
    msg.json() = _network->getSelfInfo().json();
    msg.preparePayload();
    _network->messageNeighbors(msg);
}

void keyser::Server::verack(std::shared_ptr<Peer> peer)
{
    Message newMsg(MsgTypes::Verack);
    newMsg.json()["Outbound version"] = _network->getSelfInfo()._version;
    newMsg.json()["Outbound alias"]   = _network->getSelfInfo()._alias;
    newMsg.json()["Outbound port"]    = _network->getSelfInfo()._port;
    newMsg.json()["address"]          = peer->getEndpoint().address().to_string();

    newMsg.preparePayload();
    _network->message(peer, newMsg);
}

void keyser::Server::nodeInfoStream(std::shared_ptr<Peer> peer)
{
    Message msg(MsgTypes::NodeInfoList);

    msg.json() = nlohmann::json::array();

    for (auto& nodeInfo : _network->activeNodeList())
        msg.json().push_back(nodeInfo.json());

    msg.preparePayload();
    _network->message(peer, msg);
}

void keyser::Server::sendHeaders(std::shared_ptr<Peer> peer)
{
    // TODO - headers only
    Message msg(MsgTypes::Headers);
}

void keyser::Server::inv(std::shared_ptr<Peer> peer, int startingBlock)
{
    Message msg(MsgTypes::Inv);

    if (startingBlock == _node->chain()->getHeight() - 1)
    {
        _network->message(peer, msg);
        return;
    }
    
    for (int i = startingBlock + 1 ; i < _node->chain()->getHeight() ; i++)
    {
        msg.json()["blockIndexes"].push_back(i);
    }

    msg.preparePayload();

    _network->message(peer, msg);
}

void keyser::Server::sendBlocks(std::shared_ptr<Peer> peer, int blockIndex)
{
    Message msg(MsgTypes::Block);
    msg.json() = _node->chain()->blocks().at(blockIndex)->json();
    msg.preparePayload();
    _network->message(peer, msg);
}

void keyser::Server::handleVersion(std::shared_ptr<Peer> peer, Message& msg)
{
    msg.unpackPayload();

    // Save external address and add self to list of active nodes
    _network->getSelfInfo()._address = msg.json()["address"];
    _network->activeNodeList().insert(_network->getSelfInfo());

    // unpackPayload incoming node info
    peer->info()._version = msg.json()["Outbound version"];
    peer->info()._alias   = msg.json()["Outbound alias"];
    peer->info()._port    = msg.json()["Outbound port"];
    peer->info().startingHeight = msg.json()["chainHeight"];

    // Add this node to connectedNodeList
    _network->connectedNodeList().insert(peer->info());

    // Send self info as well as their external ip
    verack(peer);
}

void keyser::Server::handleGetNodeList(std::shared_ptr<Peer> peer, Message& msg)
{
    nodeInfoStream(peer);
}

void keyser::Server::handleGetBlocks(std::shared_ptr<Peer> peer, Message& msg)
{   
    msg.unpackPayload();
    std::string topBlockHash = msg.json()["topBlock"];

    int topBlockI = 0;

    for (auto& block : _node->chain()->blocks())
    {   
        if (block->hash() == topBlockHash)
            break;

        topBlockI++;
    }

    inv(peer, topBlockI);
}

void keyser::Server::handleGetData(std::shared_ptr<Peer> peer, Message& msg)
{
    msg.unpackPayload();

    for (auto i : msg.json()["blockIndexes"])
    {
        Message msg(MsgTypes::Block);
        msg.json() = _node->chain()->blocks().at(i)->json();
        msg.preparePayload();
        _network->message(peer, msg);
    }
}

bool keyser::Server::allowConnect(std::shared_ptr<Peer> peer)
{ return true; }

void keyser::Server::onIncomingConnect(std::shared_ptr<Peer> peer)
{}