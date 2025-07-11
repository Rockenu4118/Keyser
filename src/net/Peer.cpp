#include <memory>

#include "./Peer.hpp"

keyser::Peer::Peer(PeerInfo::Direction direction,
                   boost::asio::io_context& asioContext, 
                   boost::asio::ip::tcp::socket socket, 
                   tsqueue<OwnedMessage>& qMessagesIn,
                   uint16_t uid) :
                    _context(asioContext),
                   _socket(std::move(socket)),
                   _messagesIn(qMessagesIn)
{
    _info.direction = direction;
    _info.id = uid;

    if (direction == PeerInfo::Direction::Inbound)
        _endpoint = _socket.remote_endpoint();
}

keyser::Peer::~Peer()
{
    disconnect();
}

std::string keyser::Peer::toString()
{
    std::stringstream out;
    out << "[" << getId() << "] ";
    out << getEndpoint() << ", ";
    out << "Direction: " << (info().direction == PeerInfo::Direction::Outbound ? "Outbound" : "Inbound");
    return out.str();
}

uint16_t keyser::Peer::getId() const
{
    return _info.id;
}

boost::asio::ip::tcp::endpoint keyser::Peer::getEndpoint() const
{
    return _endpoint;
}

bool keyser::Peer::isConnected() const
{
    return _socket.is_open();
}

keyser::PeerInfo& keyser::Peer::info()
{
    return _info;
}

void keyser::Peer::listen()
{
    if (isConnected()) 
    {
        readHeader();
    }
}

void keyser::Peer::send(const NetMessage& msg)
{
    boost::asio::post(_context,
        [this, msg]()
        {
            bool writingMessage = !_messagesOut.empty();

            _messagesOut.pushBack(msg);

            if (!writingMessage) 
            {
                writeHeader();
            }
        }
    );
}

void keyser::Peer::readHeader()
{
    boost::asio::async_read(_socket, boost::asio::buffer(&_msgTemporaryIn.header, sizeof(NetMessageHeader)),
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
                std::cout << "[" << getEndpoint() << "] Read Header Fail." << std::endl;
                disconnect();
            }
        }
    );
}

void keyser::Peer::readBody()
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
                std::cout << "[" << getEndpoint() << "] Read Body Fail." << std::endl;
                disconnect();
            }
        }
    );
}

void keyser::Peer::writeHeader()
{
    boost::asio::async_write(_socket, boost::asio::buffer(&_messagesOut.front().header, sizeof(NetMessageHeader)),
        [this](std::error_code ec, size_t length)
        {
            if (!ec)
            {
                if (_messagesOut.front().body.size() > 0)
                {
                    writeBody();
                }
                else
                {
                    _messagesOut.popFront();

                    if (!_messagesOut.empty())
                    {
                        writeHeader();
                    }
                }
            }
            else
            {
                std::cout << "[" << getEndpoint() << "] Write Header Fail." << std::endl;
                disconnect();
            }
        }
    );
}

void keyser::Peer::writeBody()
{
    boost::asio::async_write(_socket, boost::asio::buffer(_messagesOut.front().body.data(), _messagesOut.front().body.size()),
        [this](std::error_code ec, size_t length)
        {
            if (!ec)
            {
                _messagesOut.popFront();

                if (!_messagesOut.empty())
                {
                    writeHeader();
                }
            }
            else
            {
                std::cout << "[" << getEndpoint() << "] Write Body Fail." << std::endl;
                disconnect();
            }
        }
    );
}

void keyser::Peer::addToIncomingMessageQueue()
{ 
    _messagesIn.pushBack(OwnedMessage(this->shared_from_this(), _msgTemporaryIn));
                    
    readHeader();
}

bool keyser::Peer::connect(const boost::asio::ip::tcp::endpoint& endpoint)
{
    boost::system::error_code ec;

    _socket.connect(endpoint, ec);

    if (!ec)
    {
        readHeader();
        _endpoint = _socket.remote_endpoint();
        return true;
    }
    else
    {
        std::cout << "[NODE] Failed to connect." << std::endl;
        disconnect();
        return false;
    }
}

void keyser::Peer::disconnect()
{   
    if (isConnected())
    {
        _socket.close();
    }
}

namespace keyser
{
    std::ostream& operator<<(std::ostream& out, Peer& data)
    {
        out << "[" << data.getId() << "] ";
        out << data.getEndpoint() << ", ";
        out << "Hosting on: " << data.info().endpoint.port << ", ";
        out << "Direction: " << (data.info().direction == PeerInfo::Direction::Outbound ? "Outbound" : "Inbound");

        return out;
    }
}

