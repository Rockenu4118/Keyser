#ifndef SERVER_INTERFACE_H
#define SERVER_INTERFACE_H

#include <memory>
#include <deque>
#include <boost/asio.hpp>

#include "./Connection.hpp"
#include "./net_message.hpp"
#include "./tsqueue.hpp"

namespace net_core
{
    template <typename T>
    class Server_Interface
    {
        public:
            Server_Interface(uint16_t port) : _acceptor(_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
            {}

            virtual ~Server_Interface()
            {
                stop();
            }

            bool start()
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

            void stop()
            {
                // Request the context to close
                _context.stop();

                // Join the context thread
                if (_thrContext.joinable()) {
                    _thrContext.join();
                }

                std::cout << "[SERVER] Stopped!" << std::endl;
            }

            // Async
            void waitForClientConnection()
            {
                _acceptor.async_accept(
                    [this](std::error_code ec, boost::asio::ip::tcp::socket socket)
                    {
                        if (!ec) {
                            std::cout << "[SERVER] New Connection: " << socket.remote_endpoint() << std::endl;

                            std::shared_ptr<Connection<T>> newConn = 
                                std::make_shared<Connection<T>>(Connection<T>::owner::server, _context, std::move(socket), _qMessagesIn);

                            // Give user server a chance to deny connection
                            if (onClientConnect(newConn)) {
                                // Connection allowed, connection pushed to back of connection container
                                _deqConnections.push_back(std::move(newConn));

                                _deqConnections.back()->connectToClient(_idCounter++);

                                std::cout << "[" << _deqConnections.back()->getId() << "] Connection Approved" << std::endl;
                            } else {
                                std::cout << "[-----] Connection Denied" << std::endl;
                            }
                        } else {
                            // Error during acceptance
                            std::cout << "[SERVER] New Connection Error: " << ec.message() << std::endl;
                        }

                        // Prime the asio context with more work, wait for another connection...
                        waitForClientConnection();
                    }
                );
            }

            // Send message to specific client
            void messageClient(std::shared_ptr<Connection<T>> client, const Message<T>& msg)
            {
                // Send message if client is connected
                if (client && client->isConnected()) 
                {
                    client->send(msg);
                } 
                else 
                {
                    // Perform disconnect logic on client if it disconnected
                    onClientDisconnect(client);
                    client.reset();
                    // Remove client from active connection deqQueue
                    _deqConnections.erase(std::remove(_deqConnections.begin(), _deqConnections.end(), client), _deqConnections.end());
                }
            }

            // Send message to all clients
            void messageAllClients(const Message<T>& msg, std::shared_ptr<Connection<T>> ignoreClient = nullptr)
            {
                bool invalidClientExists = false;

                // Iterate through all clients in deqQueue
                for (auto& client : _deqConnections)
                {
                    // Check if client is connected
                    if (client && client->isConnected()) 
                    {
                        if (client != ignoreClient) 
                        {
                            client->send(msg);
                        }
                    } 
                    else 
                    {
                        // Could not connect, assume it disconnected.
                        onClientDisconnect(client);
                        client.reset();
                        invalidClientExists = true;
                    }
                }

                // Remove all disconnected clients at once, as to not invalidate the deqQueue
                // as its being iterated through
                if (invalidClientExists) 
                {
                    _deqConnections.erase(std::remove(_deqConnections.begin(), _deqConnections.end(), nullptr), _deqConnections.end());
                }
            }

            // Allow user to explicitly limit amount of messages allowed to be handled at once
            void update(size_t maxMessages = -1, bool wait = true)
            {
                if (wait)
                {
                    _qMessagesIn.wait();
                }

                size_t messageCount = 0;
                while (messageCount < maxMessages && !_qMessagesIn.empty())
                {
                    // Grab the front message
                    auto msg = _qMessagesIn.popFront();

                    // Pass to message handler
                    // Pass pointer to client connection, as well as actual message
                    onMessage(msg._remote, msg._msg);

                    messageCount++;
                }
            }

        protected:
            virtual bool onClientConnect(std::shared_ptr<Connection<T>> client)
            {
                return true;
            }

            virtual void onClientDisconnect(std::shared_ptr<Connection<T>> client)
            {}

            virtual void onMessage(std::shared_ptr<Connection<T>> client, Message<T>& msg)
            {}

            // Thread safe queue for incoming messages
            tsqueue<OwnedMessage<T>> _qMessagesIn;

            // Container for active validated connections
            std::deque<std::shared_ptr<Connection<T>>> _deqConnections;

            // Asio context as well as its own thread to run in
            boost::asio::io_context _context;
            std::thread             _thrContext;

            // Asio acceptor
            boost::asio::ip::tcp::acceptor _acceptor;

            // Clients will be identified in the system by an id
            uint32_t _idCounter = 10000;

    };
}

#endif