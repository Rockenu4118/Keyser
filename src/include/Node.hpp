#ifndef NODE_H
#define NODE_H

#include "../chain/include/Chain.hpp"
#include "../net/include/Server.hpp"
#include "../net/include/Client.hpp"
#include "../chain/include/WalletCollection.hpp"


namespace Keyser
{
    class Node
    {
        public:
            Node();

        private:
            Chain*  _chain;
            Server* _server;
            Client* _client;
    };
}

#endif