#ifndef NET_CONNECTION_H
#define NET_CONNECTION_H

#include <condition_variable>

#include <boost/asio.hpp>

#include "./net_message.hpp"
#include "./tsqueue.hpp"



namespace net_core
{
    template <typename T>
    class Net_Connection: public std::enable_shared_from_this<Net_Connection<T>>
    {
        public:
            // Constructor and destructor
            Net_Connection(boost::asio::io_context& asioContext, 
                           boost::asio::ip::tcp::socket socket, 
                           tsqueue<OwnedMessage<T>>& qMessagesIn, 
                           std::condition_variable& cvBlocking, 
                           uint16_t uid) : 
                           _asioContext(asioContext),
                           _socket(std::move(socket)), 
                           _cvBlocking(cvBlocking), 
                           _qMessagesIn(qMessagesIn)
            {
                _id = uid;
            }

            virtual ~Net_Connection() = default;

            uint16_t getId() const
            {
                return _id;
            }

            boost::asio::ip::tcp::endpoint getEndpoint() const
            {
                return _socket.remote_endpoint();
            }

            bool isConnected() const
            {
                return _socket.is_open();
            }

            void listen()
            {
                if (_socket.is_open()) 
                {
                    readHeader();
                }
            }

            void connect(const boost::asio::ip::tcp::resolver::results_type& endpoints)
            {
                boost::asio::async_connect(_socket, endpoints,
                    [this](std::error_code ec, boost::asio::ip::tcp::endpoint endpoint)
                    {
                        if (!ec)
                        {
                            readHeader();
                        }
                        else
                        {
                            std::cout << "didnt work" << std::endl;
                            disconnect();
                        }    
                    }
                );
            }

            void disconnect()
            {
                if (isConnected())
                {
                    boost::asio::post(_asioContext, [this]() { _socket.close(); });
                }   

                _cvBlocking.notify_one();
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
                                disconnect();
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
                                disconnect();
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
                                disconnect();
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
                                disconnect();
                            }
                        }
                    );
                }

                void addToIncomingMessageQueue()
                { 
                    _qMessagesIn.pushBack(OwnedMessage<T>(this->shared_from_this(), _msgTemporaryIn));
                    
                    readHeader();
                }

        protected:
            boost::asio::ip::tcp::socket _socket;
            boost::asio::io_context&     _asioContext;
            tsqueue<Message<T>>          _qMessagesOut;
            tsqueue<OwnedMessage<T>>&    _qMessagesIn;
            Message<T>                   _msgTemporaryIn;

            // Notify Node to delete connection if it disconnects
            std::condition_variable& _cvBlocking;

            uint16_t _id = 0;
    };
}

#endif