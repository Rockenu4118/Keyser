#ifndef PEER_H
#define PEER_H

#include "./Message.hpp"
#include "./OwnedMessage.hpp"
#include "../node/NodeInfo.hpp"


namespace keyser
{
    class OwnedMessage;

    class Peer : public std::enable_shared_from_this<Peer>
    {
        public:
            Peer(NodeInfo::Direction direction,
                 boost::asio::io_context& asioContext, 
                 boost::asio::ip::tcp::socket socket, 
                 tsqueue<OwnedMessage>& qMessagesIn, 
                 uint16_t uid);

            uint16_t getId() const;
            bool isConnected() const;
            boost::asio::ip::tcp::endpoint getEndpoint() const;

            NodeInfo& info();

            bool connect(const boost::asio::ip::tcp::endpoint& endpoints);
            void disconnect();

            void listen();
            void send(const Message& msg);

        private:
            void readHeader();
            void readBody();
            void writeHeader();
            void writeBody();
            void addToIncomingMessageQueue();

            uint16_t _id = 0;

            boost::asio::ip::tcp::socket _socket;
            boost::asio::io_context&     _context;
            tsqueue<Message>             _messagesOut;
            tsqueue<OwnedMessage>&       _messagesIn;
            Message                      _msgTemporaryIn;

            NodeInfo _info;
    };
}

#endif