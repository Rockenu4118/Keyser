#ifndef NODE_H
#define NODE_H

#include "../chain/Chain.hpp"
#include "../net/Server.hpp"
#include "../net/Client.hpp"
#include "../chain/Transaction.hpp"
#include "../chain/Block.hpp"


namespace keyser
{
    namespace node
    {
        class Node
        {
            public:
                enum class NodeType
                {
                    FullNode,
                    WalletNode
                };

                Node(NodeType type, int port);

                void start();
                void updateServerMessages();
                void updateClientMessages();

                void beginMining();
                void miningSequence();

                void sendTransaction(Transaction& transaction);
                void sendBlock(Block& block);
                void ping();
                void messageAll();

            private:
                Chain*       _chain  = nullptr;
                net::Server* _server = nullptr;
                net::Client* _client = nullptr;
                NodeType     _nodeType;

                std::thread _serverResponseThr;
                std::thread _clientResponseThr;
                std::thread _miningThr;
        };
    }
}

#endif