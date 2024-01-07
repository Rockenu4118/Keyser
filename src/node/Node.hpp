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

            void beginMining(bool continuous);
            void mineSingleBlock();
            void mineContinuously();

            void ping();

            void distributeNodeInfo(NodeInfo& nodeInfo);
            void distributeBlock(Block& block);
            void distributeTransaction(Transaction& transaction);

            void nodeInfoReq();
            void nodeInfoRes();

            void chainReq();
            void chainRes();

            void activeNodeListReq();
            void activeNodeListRes();

            // void firstTimeSetup();

            void sendActiveNodeList();
            void InitBlockDownload();

            void sendNodeInfo(NodeInfo& nodeInfo);
            void sendTransaction(Transaction& transaction);
            void sendBlock(Block& block);
            

            Chain* chain();

        protected:
            virtual void onOutgoingConnect(std::shared_ptr<Connection> connection);
            virtual bool allowConnect(std::shared_ptr<Connection> connection);
            virtual void onIncomingConnect(std::shared_ptr<Connection> connection);
            virtual void onDisconnect(std::shared_ptr<Connection> connection);
            virtual void onMessage(std::shared_ptr<Connection> connection, Message& msg);

        private:
            // Msg Handlers
            void handlePing(std::shared_ptr<Connection> connection, Message& msg);
            void handleDistributeNodeInfo(std::shared_ptr<Connection> connection, Message& msg);
            void handleDistributeBlock(std::shared_ptr<Connection> connection, Message& msg);
            void handleDistributeTransaction(std::shared_ptr<Connection> connection, Message& msg);
    
            void handleNodeInfoReq(std::shared_ptr<Connection> connection, Message& msg);
            void handleNodeInfoRes(std::shared_ptr<Connection> connection, Message& msg);
            
            void handleInitBlockDownload(std::shared_ptr<Connection> connection, Message& msg);

            // Node members
            Chain* _chain = nullptr;
            
            std::thread _miningThr;

            bool _miningStatus = false;
        };
}

#endif