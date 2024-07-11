#include <chrono>
#include <thread>

#include "./NetInterface.hpp"
#include "../data/version.hpp"
#include "../net/MsgTypes.hpp"



keyser::NetInterface::NetInterface(Node* node, uint16_t port) : _node(node)
{
    _client = std::make_shared<Client>(node, this, _context, _contextThr);

    // Run thread to handle messsages
    _responseThread = std::thread([this]() { update(); });

    // Save self info
    _selfInfo.version = KEYSER_VERSION;
    _selfInfo.alias   = "Keyser Node";
    _selfInfo.address = "Unknown";
    _selfInfo.port    = port;
}

keyser::NetInterface::~NetInterface()
{
    _context.stop();

    if (_contextThr.joinable())
        _contextThr.join();

    if (_responseThread.joinable())
        _responseThread.join();

    std::cout << "[NODE] Offline!" << std::endl;
}

void keyser::NetInterface::startServer()
{
    if (_server.use_count() != 0)
        return;

    _server = std::make_shared<Server>(_node, this, _selfInfo.port, _context, _contextThr);
    _server->start();
}

void keyser::NetInterface::message(std::shared_ptr<Peer> peer, const Message& msg)
{
    // Send message if peer is connected
    if (validateConnection(peer))
        peer->send(msg);
}

void keyser::NetInterface::messageNeighbors(const Message& msg, std::shared_ptr<Peer> ignorePeer)
{
    // Iterate through connections
    for (auto& peer : _peers)
    {
        if (validateConnection(peer))
        {
            if (peer != ignorePeer)
                peer->send(msg);
        }
    }
}

void keyser::NetInterface::managePeers()
{
    // Try connecting to peers until either an adequate number of connections have been made
    for (const auto& pair : _listeningNodes)
    {
        if (connectionCount() >= 3)
            return;

        _client->connect(pair.second);
    }
}

void keyser::NetInterface::ping()
{
    Message msg(MsgTypes::Ping);
    messageNeighbors(msg);
}

void keyser::NetInterface::pong() {}

bool keyser::NetInterface::validateConnection(std::shared_ptr<Peer>& peer)
{
    if (peer && peer->isConnected())
        return true;
    
    onDisconnect(peer);

    peer.reset();

    _peers.erase(std::remove(_peers.begin(), _peers.end(), nullptr), _peers.end());

    return false;
}

void keyser::NetInterface::update(uint8_t maxMessages, bool wait)
{
    while(1)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        
        if (_node->shutdownFlag())
            break;

        uint8_t msgCount = 0;

        while (msgCount < maxMessages && !_messagesIn.empty())
        {
            // Grab front msg
            auto msg = _messagesIn.popFront();
                    
            // Pass to msg handler
            // Pass pointer to connection, as well as actual msg
            onMessage(msg._remote, msg._msg);

            msgCount++;
        }
    }
}

int keyser::NetInterface::connectionCount() const
{
    return _peers.size();
}

keyser::PeerInfo keyser::NetInterface::getSelfInfo() const
{
    return _selfInfo;
}

std::shared_ptr<keyser::Peer> keyser::NetInterface::syncNode()
{
    // TODO - potentially decide which node should be the sync node
    return _peers.front();
}

uint16_t keyser::NetInterface::assignId()
{
    return _idCounter++;
}

void keyser::NetInterface::displayPeers()
{
    if (connectionCount() == 0)
    {
        std::cout << "No connections." << std::endl;
        return;
    }

    for (auto& peer : _peers)
    {
        if (validateConnection(peer))
            std::cout << *peer << std::endl;
    }
}

void keyser::NetInterface::displayListeningNodes()
{
    if (_listeningNodes.size() == 0)
    {
        std::cout << "No active nodes." << std::endl;
        return;
    }

    for (auto& pair : _listeningNodes)
    {
        std::cout << pair.second.address << ":" << pair.second.port << std::endl;
    }
}

void keyser::NetInterface::displaySelfInfo()
{
    std::cout << "Alias:   " << _selfInfo.alias   << std::endl;
    std::cout << "Address: " << _selfInfo.address << std::endl;
    std::cout << "Port:    " << _selfInfo.port    << std::endl;
}

std::vector<keyser::PeerInfo> keyser::NetInterface::getConnections() const
{
    std::vector <keyser::PeerInfo> connections;

    for (auto& peer : _peers)
        connections.push_back(peer->info());

    return connections;
}

void keyser::NetInterface::distributePeerInfo(PeerInfo& peerInfo)
{
    Message msg(MsgTypes::DistributeNodeInfo);
    msg.json() = peerInfo.json();
    msg.preparePayload();
    messageNeighbors(msg);
}

void keyser::NetInterface::distributeTransaction(Transaction& transaction)
{
    Message msg(MsgTypes::DistributeTransaction);
    msg.json() = transaction.json();
    msg.preparePayload();
    messageNeighbors(msg);
}

void keyser::NetInterface::distributeBlock(Block& block)
{
    Message msg(MsgTypes::DistributeBlock);
    msg.json() = block.json();
    msg.preparePayload();
    messageNeighbors(msg);
}

void keyser::NetInterface::onDisconnect(std::shared_ptr<Peer> peer)
{}

void keyser::NetInterface::onMessage(std::shared_ptr<Peer> peer, Message& msg)
{
    switch (msg.header.id)
    {
        case MsgTypes::Generic:
            std::cout << "[NODE] Generic msg" << std::endl;
            break;
        case MsgTypes::Ping:
            handlePing(peer, msg);
            break;
        case MsgTypes::DistributeNodeInfo:
            handleDistributePeerInfo(peer, msg);
            break;
        case MsgTypes::DistributeBlock:
            handleDistributeBlock(peer, msg);
            break;
        case MsgTypes::DistributeTransaction:
            handleDistributeTransaction(peer, msg);
            break;
        case MsgTypes::Version:
            _server->handleVersion(peer, msg);
            break;
        case MsgTypes::Verack:
            _client->handleVerack(peer, msg);
            break;
        case MsgTypes::GetBlocks:
            _server->handleGetBlocks(peer, msg);
            break;
        case MsgTypes::Inv:
            _client->handleInv(peer, msg);
            break;
        case MsgTypes::GetData:
            _server->handleGetData(peer, msg);
            break;
        case MsgTypes::Block:
            _client->handleBlock(peer, msg);
            break;
        case MsgTypes::GetNodeList:
            _server->handleGetNodeList(peer, msg);
            break;
        case MsgTypes::NodeInfoList:
            _client->handlePeerInfo(peer, msg);
            break;
        case MsgTypes::GetHeaders:

            break;
        default:
            std::cout << "[NODE] Unknown Msg Type" << std::endl;
            break;
    }
}

void keyser::NetInterface::handlePing(std::shared_ptr<Peer> peer, Message& msg)
{
    std::cout << utils::localTimestamp() << "Ping: " << peer->getId() << std::endl;
}

void keyser::NetInterface::handleDistributePeerInfo(std::shared_ptr<Peer> peer, Message& msg)
{
    msg.unpackPayload();
    PeerInfo peerInfo(msg.json());

    auto iter = _listeningNodes.find(peerInfo.endpoint());
    if (iter != nullptr)
        return;

    _listeningNodes[peerInfo.endpoint()] = peerInfo;

    managePeers();

    messageNeighbors(msg, peer);
}

void keyser::NetInterface::handleDistributeTransaction(std::shared_ptr<Peer> peer, Message& msg)
{
    msg.unpackPayload();
    Transaction transaction(msg.json());

    if (_node->validationEngine()->validateTransaction(transaction))
        messageNeighbors(msg, peer);
}

void keyser::NetInterface::handleDistributeBlock(std::shared_ptr<Peer> peer, Message& msg)
{
    msg.unpackPayload();
    auto block = std::make_shared<Block>(msg.json());

    if (_node->validationEngine()->validateBlock(block))
        messageNeighbors(msg, peer);
}

std::shared_ptr<keyser::Client>& keyser::NetInterface::client()
{
    return _client;
}

std::shared_ptr<keyser::Server>& keyser::NetInterface::server()
{
    return _server;
}

std::deque<std::shared_ptr<keyser::Peer>>& keyser::NetInterface::peers()
{
    return _peers;
}

std::unordered_map<std::string, keyser::PeerInfo>& keyser::NetInterface::listeningNodes()
{
    return _listeningNodes;
}

keyser::tsqueue<keyser::OwnedMessage>& keyser::NetInterface::messagesIn()
{
    return _messagesIn;
}