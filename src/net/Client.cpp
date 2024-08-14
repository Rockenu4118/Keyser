#include "./Client.hpp"


keyser::Client::Client(Node* node, NetInterface* network, boost::asio::io_context& context, std::thread& contextThr)
    : _node(node), _network(network), _context(context), _contextThr(contextThr)
{}

keyser::Client::~Client()
{
    _clientContext.stop();

    if (_clientContextThr.joinable())
        _clientContextThr.join();
}

bool keyser::Client::connect(Endpoint endpoint)
{   
    if (!allowConnect(endpoint))
        return false;

    try
    {
        boost::asio::ip::tcp::endpoint ep(boost::asio::ip::make_address(endpoint.address), endpoint.port);

        // Create connection
        auto newConn = std::make_shared<Peer>(PeerInfo::Direction::Outbound, _clientContext, boost::asio::ip::tcp::socket(_clientContext), _network->messagesIn(), _network->assignId());

        // Tell connection object to connect, if failed return false and reset connection
        if (!newConn->connect(ep))
        {
            newConn.reset();
            return false;
        }

        if (!_clientContextThr.joinable())
            _clientContextThr = std::thread([this]() { _clientContext.run(); });

        newConn->info().endpoint.address = newConn->getEndpoint().address().to_string();
        newConn->info().endpoint.port    = newConn->getEndpoint().port();

        // Connection moved to back of connection container
        _network->peers().push_back(std::move(newConn));

        onOutgoingConnect(_network->peers().back());
    }
    catch (const std::exception& e)
    {
        std::cerr << "[CLIENT] Exception: " << e.what() << std::endl;
        return false;
    }

    return true;
}

bool keyser::Client::allowConnect(Endpoint endpoint) const
{
    if (_network->selfInfo().endpoint == endpoint)
        return false;

    // if (_network->connectedNodeList().count(nodeInfo) == 1)
    //     return false;

    return true;
}

void keyser::Client::version(std::shared_ptr<Peer> peer)
{
    // Send self info as well as their external address
    Message msg(MsgTypes::Version);
    msg.json()["Outbound version"] = _network->selfInfo().version;
    msg.json()["Outbound port"]    = _network->selfInfo().endpoint.port;
    msg.json()["address"]          = peer->getEndpoint().address().to_string();
    
    msg.preparePayload();
    _network->message(peer, msg);
}

void keyser::Client::getNodeList()
{
    Message msg(MsgTypes::GetNodeList);
    _network->message(_network->syncNode(), msg);
}

void keyser::Client::getHeaders()
{
    Message msg(MsgTypes::GetHeaders);

    std::string hash = std::prev(_node->chain()->blockIndex().end())->second;

    msg.json()["latestHash"] = hash;

    msg.preparePayload();

    _network->message(_network->syncNode(), msg);
}

void keyser::Client::getData(DataTypes type, std::string hash)
{
    Message msg(MsgTypes::GetData);
    msg.json()["type"] = type;
    msg.json()["hash"] = hash;

    msg.preparePayload();
    _network->message(_network->syncNode(), msg);
}

void keyser::Client::completedInitialBlockDownload()
{
    _node->status(Node::Status::Online);
}

void keyser::Client::handleVerack(std::shared_ptr<Peer> peer, Message& msg)
{    
    msg.unpackPayload();

    // Save external address
    _network->selfInfo().endpoint.address = msg.json()["address"];

    // unpackPayload incoming node info
    peer->info().version = msg.json()["Outbound version"];
    
    getHeaders();
    
    // getNodeList();
}

void keyser::Client::handlePeerInfo(std::shared_ptr<Peer> peer, Message& msg)
{
    msg.unpackPayload();

    for (const auto& info : msg.json())
    {
        PeerInfo peerInfo(info);
        _network->listeningNodes()[peerInfo.endpoint.string()] = peerInfo;
    }

    _network->managePeers();
}

void keyser::Client::handleHeaders(std::shared_ptr<Peer> peer, Message& msg)
{
    msg.unpackPayload();

    for (const auto& headerJson : msg.json())
    {
        BlockHeader header(headerJson);
        
        if (_node->validationEngine()->validateHeader(header))
            getData(DataTypes::Block, header.hash());
    }
}

void keyser::Client::handleInv(std::shared_ptr<Peer> peer, Message& msg)
{
    msg.unpackPayload();

    // if (msg.json()["blockIndexes"].size() == 0)
    // {
    //     completedInitialBlockDownload();
    //     return;
    // }

    // for (int i : msg.json()["blockIndexes"])
    // {
    //     _node->chain()->inventory().push_back(i);
    // }

    // getData();
}

void keyser::Client::handleBlock(std::shared_ptr<Peer> peer, Message& msg)
{
    msg.unpackPayload();

    Block block(msg.json());

    _node->validationEngine()->validateBlock(block);
}

void keyser::Client::onOutgoingConnect(std::shared_ptr<Peer> peer)
{
    version(peer);
}
