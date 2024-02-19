#ifndef CONNECTION_H
#define CONNECTION_H

#include <condition_variable>
#include <boost/asio.hpp>
#include <tsqueue.hpp>

#include "./Message.hpp"


namespace keyser
{
    class Connection: public std::enable_shared_from_this<Connection>
    {
        // IO Stream operator
        friend std::ostream& operator<<(std::ostream& out, Connection& data);

        public:
            enum class Direction : uint8_t
            {
                Outbound,
                Inbound
            };

            // Constructor and destructor
            Connection(Direction direction,
                       boost::asio::io_context& asioContext, 
                       boost::asio::ip::tcp::socket socket, 
                       tsqueue<OwnedMessage>& qMessagesIn, 
                       std::condition_variable& cvBlocking, 
                       uint16_t uid);

            virtual ~Connection() = default;

            uint16_t getId() const;
            Direction getDirection() const;
            boost::asio::ip::tcp::endpoint getEndpoint() const;
            uint16_t getHostingPort() const;
            void setHostingPort(uint16_t port);
            uint getChainHeight();
            void setChainHeight();

            bool isConnected() const;
            void listen();
            bool connect(const boost::asio::ip::tcp::endpoint& endpoints);
            void disconnect();
            void send(const Message& msg);

        private:
            void readHeader();
            void readBody();
            void writeHeader();
            void writeBody();
            void addToIncomingMessageQueue();

        protected:
            Direction _direction;

            boost::asio::ip::tcp::socket _socket;
            boost::asio::io_context&     _asioContext;
            tsqueue<Message>             _qMessagesOut;
            tsqueue<OwnedMessage>&       _qMessagesIn;
            Message                      _msgTemporaryIn;

            uint16_t _hostingPort;
            uint     _chainHeight;

            // Notify Node to delete connection if it disconnects
            std::condition_variable& _cvBlocking;

            uint16_t _id = 0;
    };
}

#endif