#ifndef NODE_INTERFACE_H
#define NODE_INTERFACE_H

#include <set>
#include <string>
#include <mutex>
#include <condition_variable>

#include <boost/asio.hpp>

#include <tsqueue.hpp>

#include "../net/Connection.hpp"
#include "../net/Message.hpp"


namespace keyser
{
    template <typename T>
    class Connection;

    template <typename T>
    class Node_Interface
    {
        public:
            Node_Interface(uint16_t port) : _acceptor(_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
            {
                // Save port number
                _port = port;

                // Run thread to handle messsages
                _responseThread = std::thread([this]() { update(); }); 
            }
            
            virtual ~Node_Interface() { shutdown(); } 

            bool start()
            {
                try
                {
                    // Issue work before running context to keep it active
                    waitForConnection();

                    // Run the context in its own thread
                    _contextThread = std::thread([this]() { _context.run(); });
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

            bool connect(const std::string& host, const uint16_t port)
            {
                // TODO - dont allow multiple connections to same host
                try
                {
                    // Resolve hostname/ip-address into physical address
                    boost::asio::ip::tcp::resolver               resolver(_context);
                    boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(host, std::to_string(port));

                    // Create connection
                    std::shared_ptr<Connection<T>> newConn = std::make_shared<Connection<T>>(_context, boost::asio::ip::tcp::socket(_context), _messagesIn, _cvBlocking, _idCounter++);

                    // Connection pushed to back of connection container
                    _connections.push_back(std::move(newConn));

                    // Tell connection object to connect
                    _connections.back()->connect(endpoints);
                }
                catch (std::exception& e)
                {
                    std::cerr << "[NODE] Exception: " << e.what() << std::endl;
                    return false;
                }

                return true;
            }

            void shutdown()
            {   
            //     // Request the context to close
            //     _context.stop();

            //     // Join the context thread
            //     if (_contextThread.joinable())
            //         _contextThread.join();

            //     std::cout << "[NODE] Stopped!" << std::endl;
            }

            void waitForConnection()
            {
                _acceptor.async_accept(
                    [this](std::error_code ec, boost::asio::ip::tcp::socket socket)
                    {
                        if (!ec)
                        {
                            std::cout << "[NODE] New connection: " << socket.remote_endpoint() << std::endl;

                            std::shared_ptr<Connection<T>> newConn =
                                std::make_shared<Connection<T>>(_context, std::move(socket), _messagesIn, _cvBlocking, _idCounter++);

                            // Give node a chance to deny connection
                            if (onConnect(newConn))
                            {
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
                        waitForConnection();
                    }
                );
            }

            void message(std::shared_ptr<Connection<T>> connection, const Message<T>& msg)
            {
                // Send message if peer is connected
                if (connection && connection->isConnected())
                {
                    connection->send(msg);
                }
                else
                {
                    onDisconnect(connection);
                    connection.reset();
                    _connections.erase(std::remove(_connections.begin(), _connections.end(), connection), _connections.end());
                }
            }

            void messageNeighbors(const Message<T>& msg, std::shared_ptr<Connection<T>> ignoreConnection = nullptr)
            {
                bool invalidConnectionExists = false;

                // Iterate through connections
                for (auto& connection : _connections)
                {
                    if (connection && connection->isConnected())
                    {
                        if (connection != ignoreConnection)
                        {
                            connection->send(msg);
                        }
                    }
                    else
                    {
                        onDisconnect(connection);
                        connection.reset();
                        invalidConnectionExists = true;
                    }
                }

                if (invalidConnectionExists) 
                {
                    _connections.erase(std::remove(_connections.begin(), _connections.end(), nullptr), _connections.end());
                }
            }

            void removeInvalidConnections()
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

            // Handle incoming messages
            void update(uint8_t maxMessages = -1, bool wait = true)
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

            void displayConnections()
            {
                if (_connections.size() == 0)
                {
                    std::cout << "No connections." << std::endl;
                }

                for (auto& connection : _connections)
                {
                    std::cout << "[" << connection->getId() << "] " << connection->getEndpoint() << std::endl;
                }
            }

        protected:
            virtual bool onConnect(std::shared_ptr<Connection<T>> connection)
            { return true; }

            virtual void onDisconnect(std::shared_ptr<Connection<T>> connection)
            {}

            virtual void onMessage(std::shared_ptr<Connection<T>> connection, Message<T>& msg)
            {}

            // Thread safe queue for incoming messages
            tsqueue<OwnedMessage<T>> _messagesIn;
            
            // Thread for handling incoming messages
            std::thread _responseThread;

            // Container for active validated connections
            std::deque<std::shared_ptr<Connection<T>>> _connections{};

            // Addresses of active nodes on network
            // std::set<std::string> _activeAddrList;


            // Asio context as well as its own thread to run in
            boost::asio::io_context _context;
            std::thread             _contextThread;

            // Thread for cleaning invalid connections
            // Mutex and condition variable for blocking thread
            // until connection either fails or disconnects
            std::condition_variable _cvBlocking;
            std::mutex              _muxBlocking;
            std::thread             _connectionRemovalThread;

            // Asio acceptor
            boost::asio::ip::tcp::acceptor _acceptor;

            // Current port server is running on
            uint16_t _port;

            // Connections will be identified in the system by an id
            uint16_t _idCounter = 10000;
    };
}


#endif