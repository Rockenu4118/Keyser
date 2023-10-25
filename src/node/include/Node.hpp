#ifndef FULL_NODE_H
#define FULL_NODE_H

#include "../../chain/include/Chain.hpp"
#include "../../net/include/Server.hpp"
#include "../../net/include/Client.hpp"
#include "../../chain/include/WalletCollection.hpp"
#include "../../chain/include/Transaction.hpp"


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

            void sendTransaction(Transaction transaction);
            void ping();

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