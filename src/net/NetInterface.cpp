#include <chrono>
#include <thread>
#include <set>
#include <vector>
#include <string>
#include <mutex>
#include <condition_variable>
#include <boost/asio.hpp>
#include <tsqueue.hpp>

#include "../data/version.hpp"
#include "../net/MsgTypes.hpp"
#include "./NetInterface.hpp"
#include "../net/Message.hpp"
#include "../node/NodeInfo.hpp"


keyser::NetInterface::NetInterface(uint16_t port, bool& shutdownNode) : _acceptor(_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)), _shutdownNode(shutdownNode)
{
    // Close acceptor until server starts
    _acceptor.close();

    // Save self info
    _selfInfo._version = KEYSER_VERSION;
    _selfInfo._alias   = "Keyser Node";
    _selfInfo._address = "";
    _selfInfo._port    = port;
}

keyser::NetInterface::~NetInterface()
{
    shutdown();
}

bool keyser::NetInterface::startServer()
{
    try
    {
        // Setup server to be able to accept connections
        boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), _selfInfo._port);
        _acceptor.open(endpoint.protocol());
        _acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
        _acceptor.bind(endpoint);
        _acceptor.listen();

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

void keyser::NetInterface::shutdown()
{   
    // Request the context to close
    _context.stop();

    // Join the context thread
    if (_contextThread.joinable())
        _contextThread.join();

    if (_responseThread.joinable())
        _responseThread.join();

    std::cout << "[NODE] Stopped!" << std::endl;
}

bool keyser::NetInterface::connect(const NodeInfo nodeInfo)
{   
    if (_selfInfo == nodeInfo)
        return false;

    if (_connectedNodeList.count(nodeInfo) == 1)
    {
        return false;
    }   

    try
    {
        boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::make_address(nodeInfo._address), nodeInfo._port);

        // Create connection
        auto newConn = std::make_shared<Peer>(NodeInfo::Direction::Outbound, _context, boost::asio::ip::tcp::socket(_context), _messagesIn, _idCounter++);

        // Tell connection object to connect, if failed return false and reset connection
        if (!newConn->connect(endpoint))
        {
            newConn.reset();
            return false;
        }

        // Connection moved to back of connection container
        _peers.push_back(std::move(newConn));

        onOutgoingConnect(_peers.back());
    }
    catch (std::exception& e)
    {
        std::cerr << "[NODE] Exception: " << e.what() << std::endl;
        return false;
    }

    return true;
}

void keyser::NetInterface::acceptConnection()
{
    _acceptor.async_accept(
        [this](std::error_code ec, boost::asio::ip::tcp::socket socket)
        {
            if (!ec)
            {
                std::cout << "[NODE] New connection: " << socket.remote_endpoint() << std::endl;

                auto newConn = std::make_shared<Peer>(NodeInfo::Direction::Inbound, _context, std::move(socket), _messagesIn, _idCounter++);

                // Give node a chance to deny connection
                if (allowConnect(newConn))
                {
                    onIncomingConnect(newConn);

                    // Listen for message
                    newConn->listen();
                    
                    // Connection allowed, connection pushed to back of connection container
                    _peers.push_back(std::move(newConn));

                    std::cout << "[" << _peers.back()->getId() << "] Connection Approved" << std::endl;
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
    // or there are no more potential connections
    while ((connectionCount() <= 3) && (!_potentialConnections.empty()))
    {
        // Grab info from front of deque
        NodeInfo nodeInfo = _potentialConnections.front();
        _potentialConnections.pop_front();
            
        connect(nodeInfo);
    }
}

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
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        
        if (_shutdownNode)
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

std::shared_ptr<keyser::Peer> keyser::NetInterface::syncNode()
{
    // TODO - potentially decide which node should be the sync node
    return _peers.front();
}

void keyser::NetInterface::displayConnections()
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

void keyser::NetInterface::displayActiveNodes()
{
    if (_activeNodeList.size() == 0)
    {
        std::cout << "No active nodes." << std::endl;
        return;
    }

    for (auto& nodeInfo : _activeNodeList)
    {
        std::cout << nodeInfo._address << ":" << nodeInfo._port << std::endl;
    }
}

void keyser::NetInterface::displaySelfInfo()
{
    std::cout << "Alias:   " << _selfInfo._alias   << std::endl;
    std::cout << "Address: " << _selfInfo._address << std::endl;
    std::cout << "Port:    " << _selfInfo._port    << std::endl;
}

std::vector<keyser::NodeInfo> keyser::NetInterface::getConnections() const
{
    std::vector <keyser::NodeInfo> connections;

    for (auto& peer : _peers)
        connections.push_back(peer->info());

    return connections;
}

bool keyser::NetInterface::allowConnect(std::shared_ptr<Peer> peer)
{ return true; }

void keyser::NetInterface::onOutgoingConnect(std::shared_ptr<Peer> peer)
{}

void keyser::NetInterface::onIncomingConnect(std::shared_ptr<Peer> peer)
{}

void keyser::NetInterface::onDisconnect(std::shared_ptr<Peer> peer)
{}

void keyser::NetInterface::onMessage(std::shared_ptr<Peer> peer, Message& msg)
{}