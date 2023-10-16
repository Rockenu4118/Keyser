#include <iostream>

#include "./include/Node.hpp"
#include "./chain/include/Chain.hpp"
#include "./net/include/Server.hpp"
#include "./net/include/Client.hpp"


Keyser::Node::Node()
{
    _chain  = new Chain(5, 100);
    _server = new Server(6000);
    _client = new Client();
}

