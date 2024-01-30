#ifndef NODE_H
#define NODE_H

#include "./Node_Interface.hpp"
#include "../net/Connection.hpp"

#include "../chain/Chain.hpp"
#include "../chain/Transaction.hpp"
#include "../chain/Block.hpp"
#include "../node/NodeInfo.hpp"


namespace keyser
{
    class Node : public Node_Interface
    {
        public:
            Node(uint16_t port);

            // Node actions
            void beginMining(bool continuous);
            bool createTransaction(Transaction& transaction);

            void ping();

            void getBlocks();
            void sendBlocks(std::shared_ptr<Connection> connection);

            void nodeInfoStream(std::shared_ptr<Connection> connection);
            
            Chain* chain();

        protected:
            virtual bool allowConnect(std::shared_ptr<Connection> connection);
            virtual void onIncomingConnect(std::shared_ptr<Connection> connection);
            virtual void onDisconnect(std::shared_ptr<Connection> connection);
            virtual void onMessage(std::shared_ptr<Connection> connection, Message& msg);

        private:
            // Called by beginMining
            void mineSingleBlock();
            void mineContinuously();

            // Distribution msgs only called by public functions
            void distributeNodeInfo(NodeInfo& nodeInfo);
            void distributeBlock(Block& block);
            void distributeTransaction(Transaction& transaction);

            // Msg Handlers
            void handlePing(std::shared_ptr<Connection> connection, Message& msg);
            void handlePong(std::shared_ptr<Connection> connection, Message& msg);
            void handleDistributeNodeInfo(std::shared_ptr<Connection> connection, Message& msg);
            void handleDistributeBlock(std::shared_ptr<Connection> connection, Message& msg);
            void handleDistributeTransaction(std::shared_ptr<Connection> connection, Message& msg);
            void handleVersion(std::shared_ptr<Connection> connection, Message& msg);
            void handleVerack(std::shared_ptr<Connection> connection, Message& msg);
            void handleGetBlocks(std::shared_ptr<Connection> connection, Message& msg);
            void handleBlocks(std::shared_ptr<Connection> connection, Message& msg);
            void handleGetNodeList(std::shared_ptr<Connection> connection, Message& msg);
            void handleNodeInfo(std::shared_ptr<Connection> connection, Message& msg);

            // Node members
            Chain*      _chain = nullptr;
            std::thread _miningThr;
            bool        _miningStatus = false;
        };
}

#endif