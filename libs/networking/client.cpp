#include <string>
#include <boost/asio.hpp>

#include "./client.hpp"
#include "./tsqueue.hpp"
#include "./net_message.hpp"


// Constructor and destructor

template <typename T>
networking::Client<T>::~Client()
{
    disconnect();
}

template <typename T>
bool networking::Client<T>::connect(const std::string& host, const uint16_t port)
{
    try
    {
        // Create connection
        _connection = std::make_unique<Connection<T>>();

        // Resolve hostname/ip-address into physical address
        boost::asio::ip::tcp::resolver               resolver(_context);
        boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(host, std::to_string(port));

        // Tell the connection object to connect to server
        _connection->connectToServer();

        // Start context thread
        _thrContext = std::thread([this]() { _context.run(); });

    }
    catch (std::exception& e)
    {

    }

    return false;
}

template <typename T>
void networking::Client<T>::disconnect()
{
    // Check if connection exists, if so, disconnect
    if (isConnected()) {
        _connection->disconnect();
    }

    // Stop asio context as well as its thread
    _context.stop();

    if (_thrContext.joinable()) {
        _thrContext.join();
    }

    // Destroy connection object, release unique pointer
    _connection.release();
}

template <typename T>
bool networking::Client<T>::isConnected()
{
    if (_connection) {
        return _connection->isConnected();
    } else {
        return false;
    }
}

template <typename T>
networking::tsqueue<networking::OwnedMessage<T>>& networking::Client<T>::incoming()
{
    return _qMessagesIn;
}