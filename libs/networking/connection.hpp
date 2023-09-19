#ifndef CONNECTION_H
#define CONNECTION_H

#include <boost/asio.hpp>

#include "./net_message.hpp"
#include "./tsqueue.hpp"


// Forward declare classes
template <typename T>
class Message;

template <typename T>
class OwnedMessage;


namespace networking
{
    template <typename T>
    class Connection: public std::enable_shared_from_this<Connection<T>>
    {
        public:
            // Constructor and destructor
            Connection()          = default;
            virtual ~Connection() = default;


            // Utility
            bool connectToServer();
            bool disconnect();
            bool isConnected() const;

            bool send(const Message<T>& msg);

        protected:
            boost::asio::ip::tcp::socket _socket;
            boost::asio::io_context&     _asioContext;
            tsqueue<Message<T>>          _qMessagesOut;
            tsqueue<OwnedMessage<T>>&    _qMessagesIn;

    };
}







#endif