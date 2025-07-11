#include <chrono>
#include <thread>

#include "./NetInterface.hpp"

#include "../app/App.hpp"
#include "../data/version.hpp"
#include "../net/MsgTypes.hpp"



keyser::NetInterface::NetInterface(Node* node, MainMenu* menu, uint16_t port): _acceptor(_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)), mNode(node), mMenu(menu)
{
    // Run thread to handle messsages
    _responseThread = std::thread([this]() { update(); });
    //
    // // Save self info
    _selfInfo.version = KEYSER_VERSION;
    _selfInfo.endpoint.address = "Unknown";
    _selfInfo.endpoint.port = port;
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

bool keyser::NetInterface::connect(Endpoint endpoint)
{
    if (!allowConnect(endpoint))
        return false;

    try
    {
        boost::asio::ip::tcp::endpoint ep(boost::asio::ip::make_address(endpoint.address), endpoint.port);

        // Create connection
        auto newConn = std::make_shared<Peer>(PeerInfo::Direction::Outbound, _context, boost::asio::ip::tcp::socket(_context), mNode->mNetwork->mMessagesIn, mNode->mNetwork->assignId());

        // Tell connection object to connect, if failed return false and reset connection
        if (!newConn->connect(ep))
        {
            newConn.reset();
            return false;
        }

        if (!_contextThr.joinable())
            _contextThr = std::thread([this]() { _context.run(); });

        newConn->info().endpoint.address = newConn->getEndpoint().address().to_string();
        newConn->info().endpoint.port    = newConn->getEndpoint().port();

        // Connection moved to back of connection container
        mNode->mNetwork->mPeers.push_back(std::move(newConn));

        onOutgoingConnect(mNode->mNetwork->mPeers.back());
    }
    catch (const std::exception& e)
    {
        std::cerr << "[CLIENT] Exception: " << e.what() << std::endl;
        return false;
    }

    return true;
}

bool keyser::NetInterface::allowConnect(Endpoint endpoint) const
{
    if (_selfInfo.endpoint == endpoint)
        return false;

    // if (_network->connectedNodeList().count(nodeInfo) == 1)
    //     return false;

    return true;
}

bool keyser::NetInterface::start()
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

        _contextThr = std::thread([this]() { _context.run(); });
    }
    catch (const std::exception& e)
    {
        std::cerr << "[SERVER] Exception: " << e.what() << std::endl;
        return false;
    }

    // announceSelf();

    mMenu->log("[SERVER] Started on port " + std::to_string(_selfInfo.endpoint.port));
    return false;
}

void keyser::NetInterface::stop()
{
    _acceptor.close();

    _context.stop();

    if (_contextThr.joinable())
        _contextThr.join();
}

void keyser::NetInterface::acceptConnection()
{
    _acceptor.async_accept(
        [this](std::error_code ec, boost::asio::ip::tcp::socket socket)
        {
            if (!ec)
            {
                // std::cout << "[SERVER] New connection: " << socket.remote_endpoint() << std::endl;
                mMenu->log("[SERVER] New connection: " + socket.remote_endpoint().address().to_string() + ":" + std::to_string(socket.remote_endpoint().port()));

                auto newConn = std::make_shared<Peer>(PeerInfo::Direction::Inbound, _context, std::move(socket), mMessagesIn, assignId());

                newConn->info().endpoint.address = newConn->getEndpoint().address().to_string();

                // Give node a chance to deny connection
                if (allowConnect(newConn))
                {
                    onIncomingConnect(newConn);

                    // Listen for message
                    newConn->listen();

                    // Connection allowed, connection pushed to back of connection container
                    mPeers.push_back(std::move(newConn));

                    // std::cout << "[" << _peers.back()->getId() << "] Connection Approved" << std::endl;
                    mMenu->log("[" + std::to_string(mPeers.back()->getId()) + "] Connection Approved");
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

void keyser::NetInterface::message(std::shared_ptr<Peer> peer, const NetMessage& msg)
{
    // Send message if peer is connected
    if (validateConnection(peer))
        peer->send(msg);
}

void keyser::NetInterface::messageNeighbors(const NetMessage& msg, std::shared_ptr<Peer> ignorePeer)
{
    // Iterate through connections
    for (auto& peer : mPeers)
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
    for (const auto&[key, info] : _listeningNodes)
    {
        if (connectionCount() >= 3)
            return;

        connect(info.endpoint);
    }
}

void keyser::NetInterface::ping()
{
    NetMessage msg;
    messageNeighbors(msg);
}

void keyser::NetInterface::pong() {}

bool keyser::NetInterface::validateConnection(std::shared_ptr<Peer>& peer)
{
    if (peer && peer->isConnected())
        return true;
    
    onDisconnect(peer);

    peer.reset();

    mPeers.erase(std::remove(mPeers.begin(), mPeers.end(), nullptr), mPeers.end());

    return false;
}

void keyser::NetInterface::update(uint8_t maxMessages, bool wait)
{
    while(true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        
        if (mShutdown)
            break;

        uint8_t msgCount = 0;

        while (msgCount < maxMessages && !mMessagesIn.empty())
        {
            // Grab front msg
            auto msg = mMessagesIn.popFront();
                    
            // Pass to msg handler
            // Pass pointer to connection, as well as actual msg
            onMessage(msg._remote, msg._msg);

            msgCount++;
        }
    }
}

int keyser::NetInterface::connectionCount() const
{
    return mPeers.size();
}

keyser::PeerInfo& keyser::NetInterface::selfInfo()
{
    return _selfInfo;
}

std::shared_ptr<keyser::Peer> keyser::NetInterface::syncNode()
{
    // TODO - potentially decide which node should be the sync node
    return mPeers.front();
}

uint16_t keyser::NetInterface::assignId()
{
    return _idCounter++;
}

std::vector<keyser::PeerInfo> keyser::NetInterface::getConnections() const
{
    std::vector <keyser::PeerInfo> connections;

    for (auto& peer : mPeers)
        connections.push_back(peer->info());

    return connections;
}

void keyser::NetInterface::inv(std::shared_ptr<Peer> peer)
{
    NetMessage msg;

    // if (startingBlock == _node->chain()->getHeight() - 1)
    // {
    //     _network->message(peer, msg);
    //     return;
    // }
    
    // for (int i = startingBlock + 1 ; i < _node->chain()->getHeight() ; i++)
    // {
    //     msg.json()["blockIndexes"].push_back(i);
    // }

    // msg.preparePayload();

    message(peer, msg);
}

void keyser::NetInterface::distributePeerInfo(PeerInfo& peerInfo)
{
    NetMessage msg;
    // msg.json() = peerInfo.json();
    // msg.preparePayload();
    messageNeighbors(msg);
}

void keyser::NetInterface::distributeTransaction(Transaction& transaction)
{
    NetMessage msg;
    // msg.json() = transaction.json();
    // msg.preparePayload();
    messageNeighbors(msg);
}

void keyser::NetInterface::distributeBlock(Block& block)
{
    NetMessage msg;
    // msg.json() = block.json();
    // msg.preparePayload();
    messageNeighbors(msg);
}

void keyser::NetInterface::onOutgoingConnect(std::shared_ptr<Peer> peer)
{
    // version(peer);
}

bool keyser::NetInterface::allowConnect(std::shared_ptr<Peer> peer)
{ return true; }

void keyser::NetInterface::onIncomingConnect(std::shared_ptr<Peer> peer)
{}

void keyser::NetInterface::onDisconnect(std::shared_ptr<Peer> peer)
{}

void keyser::NetInterface::onMessage(std::shared_ptr<Peer> peer, NetMessage& msg)
{
    // switch (msg.header.id)
    // {
    //     case MsgTypes::Generic:
    //         std::cout << "[NODE] Generic msg" << std::endl;
    //         break;
    //     case MsgTypes::Ping:
    //         handlePing(peer, msg);
    //         break;
    //     case MsgTypes::DistributeNodeInfo:
    //         handleDistributePeerInfo(peer, msg);
    //         break;
    //     case MsgTypes::DistributeBlock:
    //         handleDistributeBlock(peer, msg);
    //         break;
    //     case MsgTypes::DistributeTransaction:
    //         handleDistributeTransaction(peer, msg);
    //         break;
    //     case MsgTypes::Version:
    //         _server->handleVersion(peer, msg);
    //         break;
    //     case MsgTypes::Verack:
    //         _client->handleVerack(peer, msg);
    //         break;
    //     case MsgTypes::Inv:
    //         _client->handleInv(peer, msg);
    //         break;
    //     case MsgTypes::GetData:
    //         _server->handleGetData(peer, msg);
    //         break;
    //     case MsgTypes::Block:
    //         _client->handleBlock(peer, msg);
    //         break;
    //     case MsgTypes::GetNodeList:
    //         _server->handleGetNodeList(peer, msg);
    //         break;
    //     case MsgTypes::NodeInfoList:
    //         _client->handlePeerInfo(peer, msg);
    //         break;
    //     case MsgTypes::GetHeaders:
    //         _server->handleGetHeaders(peer, msg);
    //         break;
    //     case MsgTypes::Headers:
    //         _client->handleHeaders(peer, msg);
    //         break;
    //     default:
    //         std::cout << "[NODE] Unknown Msg Type" << std::endl;
    //         break;
    // }
}

void keyser::NetInterface::handlePing(std::shared_ptr<Peer> peer, NetMessage& msg)
{
    std::cout << utils::localTimestamp() << "Ping: " << peer->getId() << std::endl;
}

void keyser::NetInterface::handleDistributePeerInfo(std::shared_ptr<Peer> peer, NetMessage& msg)
{
    // msg.unpackPayload();
    // PeerInfo peerInfo(msg.json());
    //
    // auto iter = _listeningNodes.find(peerInfo.endpoint.string());
    //
    // if (iter != nullptr)
    //     return;
    //
    // _listeningNodes.insert(std::pair(peerInfo.endpoint.string(), peerInfo));
    //
    // managePeers();
    //
    // messageNeighbors(msg, peer);
}

void keyser::NetInterface::handleDistributeTransaction(std::shared_ptr<Peer> peer, NetMessage& msg)
{
    // msg.unpackPayload();
    // Transaction transaction(msg.json());

    // if (_node->validationEngine()->validateTransaction(transaction))
    //     messageNeighbors(msg, peer);
}

void keyser::NetInterface::handleDistributeBlock(std::shared_ptr<Peer> peer, NetMessage& msg)
{
    // msg.unpackPayload();
    // Block block = Block(msg.json());
    //
    // if (_node->validationEngine()->validateBlock(block))
    //     messageNeighbors(msg, peer);
}