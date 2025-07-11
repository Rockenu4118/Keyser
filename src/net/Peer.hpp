#ifndef PEER_H
#define PEER_H

#include <boost/asio.hpp>
#include "../utils/tsqueue.hpp"

#include "./NetMessage.hpp"
#include "./OwnedMessage.hpp"
#include "../node/PeerInfo.hpp"


namespace keyser
{
    class OwnedMessage;

    class Peer : public std::enable_shared_from_this<Peer>
    {
        // IO Stream operator
        friend std::ostream& operator<<(std::ostream& out, Peer& data);

        public:
            Peer(PeerInfo::Direction direction,
                 boost::asio::io_context& asioContext,
                 boost::asio::ip::tcp::socket socket, 
                 tsqueue<OwnedMessage>& qMessagesIn, 
                 uint16_t uid);

            ~Peer();

            std::string toString();

            uint16_t getId() const;
            bool isConnected() const;
            boost::asio::ip::tcp::endpoint getEndpoint() const;

            PeerInfo& info();

            bool connect(const boost::asio::ip::tcp::endpoint& endpoint);
            void disconnect();

            void listen();
            void send(const NetMessage& msg);

        private:
            void readHeader();
            void readBody();
            void writeHeader();
            void writeBody();
            void addToIncomingMessageQueue();

            boost::asio::ip::tcp::socket _socket;
            boost::asio::io_context&     _context;
            tsqueue<NetMessage>          _messagesOut;
            tsqueue<OwnedMessage>&       _messagesIn;
            NetMessage                   _msgTemporaryIn;

            boost::asio::ip::tcp::endpoint _endpoint;
            PeerInfo _info;
    };
}

#endif