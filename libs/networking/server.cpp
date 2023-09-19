#include <memory>
#include <utility>
#include <boost/asio.hpp>

#include "./server.hpp"
#include "./connection.hpp"


// Initialize asio acceptor with asio context as well as port number
template <typename T>
networking::Server<T>::Server(uint16_t port) : _acceptor(_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)) {}

template <typename T>
networking::Server<T>::~Server()
{
    stop();
}

template <typename T>
bool networking::Server<T>::start()
{
    try
    {
        // Issue work before running the context to keep it active
        waitForClientConnection();

        // Run the context within its own thread
        _thrContext = std::thread([this]() { _context.run(); });
    }
    catch (std::exception& e)
    {
        std::cerr << "[SERVER] Exception: " << e.what() << std::endl;
        return false;
    }

    std::cout << "[SERVER] Started!" << std::endl;
    return true;
}

template <typename T>
void networking::Server<T>::stop()
{
    // Request the context to close
    _context.stop();

    // Join the context thread
    if (_thrContext.joinable()) {
        _thrContext.join();
    }

    std::cout << "[SERVER] Stopped!" << std::endl;
}

template <typename T>
void networking::Server<T>::waitForClientConnection()
{
    _acceptor.async_accept(
        [this](std::error_code ec, boost::asio::ip::tcp::socket socket)
        {
            if (!ec) {
                std::cout << "[SERVER] New Connection: " << socket.remote_endpoint() << std::endl;

                // std::shared_ptr<Connection<T>> newConn = 
                //     std::make_shared<Connection<T>>(Connection<T>::owner::server, _context, std::move(socket), _qMessagesIn);

                // // Give user server a chance to deny connection
                // if (onClientConnect(newConn)) {
                //     // Connection allowed, connection pushed to back of connection container
                //     _deqConnections.push_back(std::move(newConn));

                //     _deqConnections.back()->ConnectToClient(_idCounter++);

                //     std::cout << "[" << _deqConnections.back()->getId() << "] Connection Approved" << std::endl;
                // } else {
                //     std::cout << "[-----] Connection Denied" << std::endl;
                // }
            } else {
                // Error during acceptance
                std::cout << "[SERVER] New Connection Error: " << ec.message() << std::endl;
            }

            // Prime the asio context with more work, wait for another connection...
            waitForClientConnection();
        }
    );
}

template <typename T>
void networking::Server<T>::messageClient(std::shared_ptr<Connection<T>> client, const Message<T>& msg)
{
    // Send message if client is connected
    if (client && client->isConnected()) {
        client->send(msg);
    } else {
        // Perform disconnect logic on client if it disconnected
        onClientDisconnect(client);
        client.reset();
        // Remove client from active connection deqQueue
        _deqConnections.erase(std::remove(_deqConnections.begin(), _deqConnections.end(), client), _deqConnections.end());
    }
}

template <typename T>
void networking::Server<T>::messageAllClients(const Message<T>& msg, std::shared_ptr<Connection<T>> ignoreClient)
{
    bool invalidClientExists = false;

    // Iterate through all clients in deqQueue
    for (auto& client : _deqConnections)
    {
        // Check if client is connected
        if (client && client->isConnected()) {
            if (client != ignoreClient) {
                client->send(msg);
            }
        } else {
            // Could not connect, assume it disconnected.
            onClientDisconnect(client);
            client.reset();
            invalidClientExists = true;
        }
    }

    // Remove all disconnected clients at once, as to not invalidate the deqQueue
    // as its being iterated through
    if (invalidClientExists) {
        _deqConnections.erase(std::remove(_deqConnections.begin(), _deqConnections.end(), nullptr), _deqConnections.end());
    }
}

template <typename T>
void networking::Server<T>::update(size_t maxMessages)
{
    size_t messageCount = 0;
    while (messageCount < maxMessages && !_qMessagesIn.empty())
    {
        // Grab the front message
        auto msg = _qMessagesIn.popFront();

        // Pass to message handler
        // Pass pointer to client connection, as well as actual message
        onMessage(msg.remote, msg.msg);

        messageCount++;
    }
}

template <typename T>
bool networking::Server<T>::onClientConnect(std::shared_ptr<Connection<T>> client)
{
    return false;
}

template <typename T>
void networking::Server<T>::onClientDisconnect(std::shared_ptr<Connection<T>> client) {}

template <typename T>
void networking::Server<T>::onMessage(std::shared_ptr<Connection<T>> client, Message<T>& msg) {}