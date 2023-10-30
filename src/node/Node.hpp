#ifndef NODE_H
#define NODE_H

#include "../chain/Chain.hpp"
#include "../net/Server.hpp"
#include "../net/Client.hpp"
#include "../chain/Transaction.hpp"


namespace keyser
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
            void ping();
            void messageAll();

        private:
            Chain*   _chain  = nullptr;
            Server*  _server = nullptr;
            Client*  _client = nullptr;
            NodeType _nodeType;

            std::thread _serverResponseThr;
            std::thread _clientResponseThr;
            std::thread _miningThr;
    };
}

#endif