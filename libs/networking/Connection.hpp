#ifndef CONNECTION_H
#define CONNECTION_H

#include <boost/asio.hpp>

#include "./net_message.hpp"
#include "./tsqueue.hpp"



namespace networking
{
    // Forward declare classes
    template <typename T>
    class Message;

    template <typename T>
    class OwnedMessage;


    template <typename T>
    class Connection: public std::enable_shared_from_this<Connection<T>>
    {
        public:
            enum class owner
            {
                server,
                client
            };

            // Constructor and destructor
            Connection(owner parent, boost::asio::io_context& asioContext, boost::asio::ip::tcp::socket socket, tsqueue<OwnedMessage<T>>& qMessagesIn)
                : _asioContext(asioContext), _socket(std::move(socket)), _qMessagesIn(qMessagesIn)
            {
                _ownerType = parent;
            }



            virtual ~Connection() = default;

            uint32_t getId() const
            {
                return _id;
            }


            // Utility
            void connectToServer()
            {}

            void disconnect()
            {}

            bool isConnected() const
            {
                return _socket.is_open();
            }

            bool send(const Message<T>& msg);

        protected:
            boost::asio::ip::tcp::socket _socket;
            boost::asio::io_context&     _asioContext;
            tsqueue<Message<T>>          _qMessagesOut;
            tsqueue<OwnedMessage<T>>&    _qMessagesIn;

            owner _ownerType = owner::server;
            uint32_t _id     = 0;

    };
}







#endif