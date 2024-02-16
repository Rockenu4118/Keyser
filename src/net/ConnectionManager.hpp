#ifndef CONNECTION_MANAGER_H
#define CONNECTION_MANAGER_H

#include <boost/asio.hpp>
#include <tsqueue.hpp>


namespace keyser
{
    class ConnectionManager
    {
        public:
            ConnectionManager(uint16_t port);

            void startServer();

        private:
            // Queue for holding node information that this node will attempt to connect to
            
            // boost::asio::io_context& _asioContext;

            // boost::asio::ip::tcp::acceptor _acceptor;
    };
}

#endif