#include <set>
#include <string>
#include <mutex>
#include <condition_variable>
#include <boost/asio.hpp>
#include <tsqueue.hpp>

#include "../data/version.hpp"
#include "../net/MsgTypes.hpp"
#include "./Node_Interface.hpp"
#include "../net/Connection.hpp"
#include "../net/Message.hpp"
#include "../node/NodeInfo.hpp"


keyser::Node_Interface::Node_Interface(uint16_t port) : _acceptor(_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
{
    // Close acceptor until server starts
    _acceptor.close();

    // Save self info
    _selfInfo._version = KEYSER_VERSION;
    _selfInfo._alias   = "whatev";
    _selfInfo._address = "";
    _selfInfo._port    = port;

    _chain         = new Chain();
    _mempool       = new Mempool();
    _storageEngine = new StorageEngine();
    _walletManager = new WalletManager();
}

keyser::Node_Interface::~Node_Interface()
{
    shutdown();
}

void keyser::Node_Interface::run()
{
    startServer();
}

bool keyser::Node_Interface::startServer()
{
    try
    {   
        // Setup server to be able to accept connections
        boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), _selfInfo._port);
        _acceptor.open(endpoint.protocol());
        _acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
        _acceptor.bind(endpoint);
        _acceptor.listen();

        // Issue work before running context to keep it active
        acceptConnection();

        // Run the context in its own thread
        _contextThread = std::thread([this]() { _context.run(); });
        // Run thread to handle messsages
        _responseThread = std::thread([this]() { update(); }); 
        // Activate thread to manage peer connections
        _peerConnectionThread = std::thread([this]() { managePeerConnections(); });
        // Activate thread to dispose of invalid connections
        _connectionRemovalThread = std::thread([this]() { removeInvalidConnections(); });
    }
    catch (std::exception& e)
    {
        std::cerr << "[NODE] Exception: " << e.what() << std::endl;
        return false; 
    }

    std::cout << "[NODE] Started!" << std::endl;
    return false;
}

bool keyser::Node_Interface::connect(const NodeInfo nodeInfo)
{   
    try
    {
        // Resolve hostname/ip-address into physical address
        // boost::asio::ip::tcp::resolver               resolver(_context);
        // boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(nodeInfo._address, std::to_string(nodeInfo._port));
        boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::make_address(nodeInfo._address), nodeInfo._port);

        // Create connection
        std::shared_ptr<Connection> newConn = 
            std::make_shared<Connection>(Connection::Direction::Outbound, _context, boost::asio::ip::tcp::socket(_context), _messagesIn, _cvBlocking, _idCounter++);

        // Connection pushed to back of connection container
        _connections.push_back(std::move(newConn));

        // Tell connection object to connect, if failed return false and remove connection
        if (!_connections.back()->connect(endpoint))
        {
            _connections.back().reset();
            _connections.erase(std::remove(_connections.begin(), _connections.end(), nullptr), _connections.end());
            return false;
        }

        onOutgoingConnect(_connections.back());
    }
    catch (std::exception& e)
    {
        std::cerr << "[NODE] Exception: " << e.what() << std::endl;
        return false;
    }

    return true;
}

void keyser::Node_Interface::shutdown()
{   
//     // Request the context to close
//     _context.stop();

//     // Join the context thread
//     if (_contextThread.joinable())
//         _contextThread.join();

//     std::cout << "[NODE] Stopped!" << std::endl;
}

void keyser::Node_Interface::acceptConnection()
{
    _acceptor.async_accept(
        [this](std::error_code ec, boost::asio::ip::tcp::socket socket)
        {
            if (!ec)
            {
                std::cout << "[NODE] New connection: " << socket.remote_endpoint() << std::endl;

                std::shared_ptr<Connection> newConn =
                    std::make_shared<Connection>(Connection::Direction::Inbound, _context, std::move(socket), _messagesIn, _cvBlocking, _idCounter++);

                // Give node a chance to deny connection
                if (allowConnect(newConn))
                {       
                    onIncomingConnect(newConn);

                    // Connection allowed, connection pushed to back of connection container
                    _connections.push_back(std::move(newConn));
                    _connections.back()->listen();

                    std::cout << "[" << _connections.back()->getId() << "] Connection Approved" << std::endl;
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

void keyser::Node_Interface::message(std::shared_ptr<Connection> connection, const Message& msg)
{
    // Send message if peer is connected
    if (connection && connection->isConnected())
        connection->send(msg);
}

void keyser::Node_Interface::messageNeighbors(const Message& msg, std::shared_ptr<Connection> ignoreConnection)
{
    // Iterate through connections
    for (auto& connection : _connections)
    {
        if (connection && connection->isConnected())
        {
            if (connection != ignoreConnection)
                connection->send(msg);
        }
    }
}

void keyser::Node_Interface::managePeerConnections()
{
    while (!_recievedNodeList)
        sleep(1);

    for (auto& nodeInfo : _activeNodeList)
    {
        if (!(_connectedNodeList.size() <= 3))
        {
            std::cout << "Alr 3 connections" << std::endl;
            return;
        }

        if (_selfInfo == nodeInfo)
        {
            std::cout << "[NODE] Cannot self connect" << std::endl;
            continue;
        }

        if (_connectedNodeList.count(nodeInfo) == 1)
        {
            std::cout << "[NODE] No duplicate connections" << std::endl;
            continue;
        }

        if (connect(nodeInfo))
        {
            _connectedNodeList.insert(nodeInfo);
        }
    }

    Message newMsg(MsgTypes::DistributeNodeInfo);
    newMsg.insert(_selfInfo);
    messageNeighbors(newMsg);
}

void keyser::Node_Interface::removeInvalidConnections()
{
    while (1)
    {
        std::unique_lock ul(_muxBlocking);
        _cvBlocking.wait(ul);
                    
        for (auto& connection : _connections)
        {
            if (!connection || !connection->isConnected())
            {   
                onDisconnect(connection);
                connection.reset();
            }
        }

        _connections.erase(std::remove(_connections.begin(), _connections.end(), nullptr), _connections.end());
    }
}

void keyser::Node_Interface::update(uint8_t maxMessages, bool wait)
{
    while(1)
    {
        if (wait)
            _messagesIn.wait();

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

void keyser::Node_Interface::displayConnections()
{
    if (_connections.size() == 0)
    {
        std::cout << "No connections." << std::endl;
        return;
    }

    for (auto& connection : _connections)
    {
        std::cout << "[" << connection->getId() << "] " << connection->getEndpoint() 
                  << ", Hosting on: " << connection->getHostingPort() 
                  << ", Direction: " << (connection->getDirection() == Connection::Direction::Outbound ? "Outbound" : "Inbound") << std::endl;
    }
}

void keyser::Node_Interface::displayActiveNodes()
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

void keyser::Node_Interface::displaySelfInfo()
{
    std::cout << _selfInfo << std::endl;
}

keyser::Chain* keyser::Node_Interface::chain()
{
    return _chain;
}

keyser::Mempool* keyser::Node_Interface::mempool()
{
    return _mempool;
}

keyser::StorageEngine* keyser::Node_Interface::storageEngine()
{
    return _storageEngine;
}

keyser::WalletManager* keyser::Node_Interface::walletManager()
{
    return _walletManager;
}

bool keyser::Node_Interface::allowConnect(std::shared_ptr<Connection> connection)
{ return true; }

void keyser::Node_Interface::onOutgoingConnect(std::shared_ptr<Connection> connection)
{}

void keyser::Node_Interface::onIncomingConnect(std::shared_ptr<Connection> connection)
{}

void keyser::Node_Interface::onDisconnect(std::shared_ptr<Connection> connection)
{}

void keyser::Node_Interface::onMessage(std::shared_ptr<Connection> connection, Message& msg)
{}