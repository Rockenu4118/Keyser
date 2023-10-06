#ifndef CONNECTION_H
#define CONNECTION_H

#include <boost/asio.hpp>

#include "./net_message.hpp"
#include "./tsqueue.hpp"



namespace networking
{
    // Forward declare classes
    template <typename T>
    class MessageHeader;

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

            void connectToClient(uint32_t uid = 0)
            {
                if (_ownerType == owner::server) 
                {
                    if (_socket.is_open()) 
                    {
                        _id = uid;
                        readHeader();
                    }
                }
            }

            void connectToServer(const boost::asio::ip::tcp::resolver::results_type& endpoints)
            {
                if (_ownerType == owner::client)
                {
                    boost::asio::async_connect(_socket, endpoints,
                        [this](std::error_code ec, boost::asio::ip::tcp::endpoint endpoint)
                        {
                            if (!ec)
                            {
                                readHeader();
                            }
                        }
                    );
                }
            }

            void disconnect()
            {
                if (isConnected())
                {
                    boost::asio::post(_asioContext, [this]() { _socket.close(); });
                }
            }

            bool isConnected() const
            {
                return _socket.is_open();
            }

            void send(const Message<T>& msg)
            {
                boost::asio::post(_asioContext,
                    [this, msg]()
                    {
                        bool writingMessage = !_qMessagesOut.empty();

                        _qMessagesOut.pushBack(msg);

                        if (!writingMessage) 
                        {
                            writeHeader();
                        }
                    }
                );
            }

            private:
                void readHeader()
                {
                    boost::asio::async_read(_socket, boost::asio::buffer(&_msgTemporaryIn.header, sizeof(MessageHeader<T>)),
                        [this](std::error_code ec, size_t length)
                        {
                            if (!ec)
                            {
                                if (_msgTemporaryIn.header.size > 0)
                                {
                                    _msgTemporaryIn.body.resize(_msgTemporaryIn.header.size);
                                    readBody();
                                }
                                else
                                {
                                    addToIncomingMessageQueue();
                                }
                            }
                            else
                            {
                                std::cout << "[" << _id << "] Read Header Fail." << std::endl;
                                _socket.close();
                            }
                        }
                    );
                }

                void readBody()
                {
                    boost::asio::async_read(_socket, boost::asio::buffer(_msgTemporaryIn.body.data(), _msgTemporaryIn.body.size()),
                        [this](std::error_code ec, size_t length)
                        {
                            if (!ec)
                            {
                                addToIncomingMessageQueue();
                            }
                            else
                            {
                                std::cout << "[" << _id << "] Read Body Fail." << std::endl;
                                _socket.close(); 
                            }
                        }
                    );
                }

                void writeHeader()
                {
                    boost::asio::async_write(_socket, boost::asio::buffer(&_qMessagesOut.front().header, sizeof(MessageHeader<T>)),
                        [this](std::error_code ec, size_t length)
                        {
                            if (!ec)
                            {
                                if (_qMessagesOut.front().body.size() > 0)
                                {
                                    writeBody();
                                }
                                else
                                {
                                    _qMessagesOut.popFront();

                                    if (!_qMessagesOut.empty())
                                    {
                                        writeHeader();
                                    }
                                }
                            }
                            else
                            {
                                std::cout << "[" << _id << "] Write Header Fail." << std::endl;
                                _socket.close(); 
                            }
                        }
                    );
                }

                void writeBody()
                {
                    boost::asio::async_write(_socket, boost::asio::buffer(_qMessagesOut.front().body.data(), _qMessagesOut.front().body.size()),
                        [this](std::error_code ec, size_t length)
                        {
                            if (!ec)
                            {
                                _qMessagesOut.popFront();

                                if (!_qMessagesOut.empty())
                                {
                                    writeHeader();
                                }
                            }
                            else
                            {
                                std::cout << "[" << _id << "] Write Body Fail." << std::endl;
                                _socket.close();
                            }
                        }
                    );
                }

                void addToIncomingMessageQueue()
                {
                    if (_ownerType == owner::server)
                    {
                        _qMessagesIn.pushBack(OwnedMessage<T>(this->shared_from_this(), _msgTemporaryIn));
                    }
                    else 
                    {
                        _qMessagesIn.pushBack(OwnedMessage<T>(nullptr, _msgTemporaryIn));
                    }

                    readHeader();
                }

        protected:
            boost::asio::ip::tcp::socket _socket;
            boost::asio::io_context&     _asioContext;
            tsqueue<Message<T>>          _qMessagesOut;
            tsqueue<OwnedMessage<T>>&    _qMessagesIn;
            Message<T>                   _msgTemporaryIn;

            owner    _ownerType = owner::server;
            uint32_t _id        = 0;
    };
}


#endif