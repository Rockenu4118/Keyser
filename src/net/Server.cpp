#include <Server_Interface.hpp>

#include "./MsgTypes.hpp"
#include "../data/version.hpp"
#include "../chain/Chain.hpp"
#include "../chain/Transaction.hpp"
#include "../chain/Block.hpp"
#include "../utils/utils.hpp"
#include "./Server.hpp"


keyser::net::Server::Server(uint16_t port, Chain* chain) : net_core::Server_Interface<MsgTypes>(port)
{
    _chain = chain;
}


