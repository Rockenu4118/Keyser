#include <boost/asio.hpp>
#include <tsqueue.hpp>

#include "./Connection.hpp"
#include "./Message.hpp"


// keyser::Connection::Connection(Direction direction,
//                                boost::asio::io_context& asioContext, 
//                                boost::asio::ip::tcp::socket socket, 
//                                tsqueue<OwnedMessage>& qMessagesIn) : 
//                                _context(asioContext),
//                                _socket(std::move(socket)), 
//                                _messagesIn(qMessagesIn)
// {
//     _direction = direction;
// }

