#include "./Client.hpp"


keyser::Client::Client(Node* node, NetInterface* network, boost::asio::io_context& context) : _node(node), _network(network), _context(context)
{}

bool keyser::Client::connect(const NodeInfo nodeInfo)
{   
    if (_network->getSelfInfo() == nodeInfo)
        return false;

    if (_network->connectedNodeList().count(nodeInfo) == 1)
    {
        return false;
    }

    try
    {
        boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::make_address(nodeInfo._address), nodeInfo._port);

        // Create connection
        auto newConn = std::make_shared<Peer>(NodeInfo::Direction::Outbound, _context, boost::asio::ip::tcp::socket(_context), _network->messagesIn(), _network->assignId());

        // Tell connection object to connect, if failed return false and reset connection
        if (!newConn->connect(endpoint))
        {
            newConn.reset();
            return false;
        }

        newConn->info()._address = newConn->getEndpoint().address().to_string();
        newConn->info()._port    = newConn->getEndpoint().port();

        // Connection moved to back of connection container
        _network->peers().push_back(std::move(newConn));

        onOutgoingConnect(_network->peers().back());
    }
    catch (std::exception& e)
    {
        std::cerr << "[NODE] Exception: " << e.what() << std::endl;
        return false;
    }

    return true;
}

void keyser::Client::version(std::shared_ptr<Peer> peer)
{
    // Send self info as well as their external address
    Message msg(MsgTypes::Version);
    msg.json()["Outbound version"] = _network->getSelfInfo()._version;
    msg.json()["Outbound alias"]   = _network->getSelfInfo()._alias;
    msg.json()["Outbound port"]    = _network->getSelfInfo()._port;
    msg.json()["chainHeight"]      = _node->chain()->getHeight();
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
    _network->message(_network->syncNode(), msg);
}

void keyser::Client::getBlocks()
{
    Message msg(MsgTypes::GetBlocks);
    msg.json()["topBlock"] = _node->chain()->getCurrBlock()->hash();
    msg.preparePayload();

    _network->message(_network->syncNode(), msg);

    _node->status(Node::Status::Syncing);
}

void keyser::Client::getData()
{
    Message msg(MsgTypes::GetData);

    for (auto element : _node->chain()->inventory())
        msg.json()["blockIndexes"].push_back(element);

    msg.preparePayload();

    _network->message(_network->syncNode(), msg);
}

void keyser::Client::completedInitialBlockDownload()
{
    _node->status(Node::Status::Online);

    // Can now advertise self as a node on the network
    _network->server()->broadcastSelf();
}

void keyser::Client::handleVerack(std::shared_ptr<Peer> peer, Message& msg)
{    
    msg.unpackPayload();

    // Save external address
    _network->getSelfInfo()._address = msg.json()["address"];

    // unpackPayload incoming node info
    peer->info()._version = msg.json()["Outbound version"];
    peer->info()._alias   = msg.json()["Outbound alias"];

    // Add info to connected node list and active node list
    _network->connectedNodeList().insert(peer->info());
    _network->activeNodeList().insert(peer->info());
    
    // Get block inv if needed
    if (!_node->chain()->blockInvRecieved())
        getBlocks();
    
    // Get node list if needed
    // if (!_recievedNodeList)
    //     getNodeList();
}

void keyser::Client::handleNodeInfo(std::shared_ptr<Peer> peer, Message& msg)
{
    msg.unpackPayload();

    for (auto& info : msg.json())
    {
        NodeInfo nodeInfo(info);
        _network->activeNodeList().insert(nodeInfo);
        _network->potentialConnections().push_back(nodeInfo);
    }

    _network->managePeers();
}

void keyser::Client::handleInv(std::shared_ptr<Peer> peer, Message& msg)
{
    msg.unpackPayload();

    _node->chain()->blockInvRecieved() = true;

    if (msg.json()["blockIndexes"].size() == 0)
    {
        completedInitialBlockDownload();
        return;
    }

    for (int i : msg.json()["blockIndexes"])
    {
        _node->chain()->inventory().push_back(i);
    }

    getData(); 
}

void keyser::Client::handleBlock(std::shared_ptr<Peer> peer, Message& msg)
{
    msg.unpackPayload();
    auto block = std::make_shared<Block>(msg.json());
    _node->validationEngine()->validateBlock(block);

    if (_node->chain()->getHeight() >= _node->chain()->inventory().back() + 1)
    {
        completedInitialBlockDownload();
    }
}

void keyser::Client::onOutgoingConnect(std::shared_ptr<Peer> peer)
{}
