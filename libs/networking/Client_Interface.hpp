#ifndef CLIENT_INTERFACE_H
#define CLIENT_INTERFACE_H

#include <string>
#include <boost/asio.hpp>

#include "./tsqueue.hpp"
#include "./Connection.hpp"
#include "./net_message.hpp"


namespace networking
{
    template <typename T>
    class Client_Interface
    {
        public:
            Client_Interface() = default;
            
            virtual ~Client_Interface()
            {
                // Attempt to disconnect from server if client is destroyed
                disconnect();
            }

            bool connect(const std::string& host, const uint16_t port)
            {
                try
                {
                    // Resolve hostname/ip-address into physical address
                    boost::asio::ip::tcp::resolver               resolver(_context);
                    boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(host, std::to_string(port));

                    // Create connection
                    _connection = std::make_unique<Connection<T>>(Connection<T>::owner::client, _context, boost::asio::ip::tcp::socket(_context), _qMessagesIn);

                    // Tell the connection object to connect to server
                    _connection->connectToServer(endpoints);

                    // Start context thread
                    _thrContext = std::thread([this]() { _context.run(); });

                }
                catch (std::exception& e)
                {
                    std::cerr << "Client Exception: " << e.what() << std::endl;
                    return false;
                }

                return true;
            }

            void disconnect()
            {
                // Check if connection exists, if so, disconnect
                if (isConnected()) 
                {
                    _connection->disconnect();
                }

                // Stop asio context as well as its thread
                _context.stop();

                if (_thrContext.joinable()) 
                {
                    _thrContext.join();
                }

                // Destroy connection object, release unique pointer
                _connection.release();
            }

            bool isConnected()
            {
                if (_connection) 
                {
                    return _connection->isConnected();
                } else 
                {
                    return false;
                }
            }

            void send(const Message<T>& msg)
            {
                if (isConnected())
                {
                    _connection->send(msg);
                }
            }

            tsqueue<OwnedMessage<T>>& incoming()
            {
                return _qMessagesIn;
            }

        protected:
            // context handles data transfer
            boost::asio::io_context _context;
            // Needs to run in its own thread to execute commands
            std::thread _thrContext;
            // Client has a single connection object to handle data transfer
            std::unique_ptr<Connection<T>> _connection;


        private:
            // Thread safe queue for incoming messages from server
            tsqueue<OwnedMessage<T>> _qMessagesIn;
    };
}

#endif