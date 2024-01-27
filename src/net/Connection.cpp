#include <condition_variable>
#include <boost/asio.hpp>
#include <tsqueue.hpp>

#include "./Connection.hpp"
#include "./Message.hpp"


keyser::Connection::Connection(boost::asio::io_context& asioContext, 
                               boost::asio::ip::tcp::socket socket, 
                               tsqueue<OwnedMessage>& qMessagesIn, 
                               std::condition_variable& cvBlocking, 
                               uint16_t uid) : 
                               _asioContext(asioContext),
                               _socket(std::move(socket)), 
                               _cvBlocking(cvBlocking), 
                               _qMessagesIn(qMessagesIn)
{
    _id = uid;
}

uint16_t keyser::Connection::getId() const
{
    return _id;
}

boost::asio::ip::tcp::endpoint keyser::Connection::getEndpoint() const
{
    return _socket.remote_endpoint();
}

bool keyser::Connection::isConnected() const
{
    return _socket.is_open();
}

void keyser::Connection::listen()
{
    if (_socket.is_open()) 
    {
        readHeader();
    }
}

bool keyser::Connection::connect(const boost::asio::ip::tcp::endpoint& endpoint)
{
    boost::system::error_code ec;

    _socket.connect(endpoint, ec);

    if (!ec)
    {
        readHeader();
        return true;
    }
    else
    {
        std::cout << "[NODE] Failed to connect." << std::endl;
        disconnect();
        return false;
    }
}

void keyser::Connection::disconnect()
{   
    if (isConnected())
    {
        _socket.close();
    }   

    _cvBlocking.notify_one();
}

void keyser::Connection::send(const Message& msg)
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

void keyser::Connection::readHeader()
{
    boost::asio::async_read(_socket, boost::asio::buffer(&_msgTemporaryIn.header, sizeof(MessageHeader)),
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

void keyser::Connection::readBody()
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

void keyser::Connection::writeHeader()
{
    boost::asio::async_write(_socket, boost::asio::buffer(&_qMessagesOut.front().header, sizeof(MessageHeader)),
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

void keyser::Connection::writeBody()
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

void keyser::Connection::addToIncomingMessageQueue()
{ 
    _qMessagesIn.pushBack(OwnedMessage(this->shared_from_this(), _msgTemporaryIn));
                    
    readHeader();
}