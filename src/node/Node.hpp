#ifndef NODE_H
#define NODE_H

#include "./NetInterface.hpp"
#include "../net/Connection.hpp"

#include "../chain/Chain.hpp"
#include "../storage/StorageEngine.hpp"
#include "../chain/Transaction.hpp"
#include "../chain/Block.hpp"
#include "../node/NodeInfo.hpp"
#include "../wallet/WalletManager.hpp"
#include "../chain/Mempool.hpp"


namespace keyser
{
    class Node : public Chain, public NetInterface
    {
        public:
            Node(uint16_t port);

            void run();

            

            // Node actions
            void beginMining(bool continuous);
            bool createTransaction(Transaction& transaction);

            // Requests and Responses
            void ping();
            void pong();

            // Initial msg sent to establish handshake from a new node
            void version(std::shared_ptr<Connection> connection);
            void verack(std::shared_ptr<Connection> connection);

            void getBlocks();
            void sendBlocks(std::shared_ptr<Connection> connection);

            void nodeInfoStream(std::shared_ptr<Connection> connection);
            

            Mempool&           mempool();
            WalletManager&     walletManager();

        protected:
            // Event handlers
            virtual bool allowConnect(std::shared_ptr<Connection> connection);
            virtual void onOutgoingConnect(std::shared_ptr<Connection> connection);
            virtual void onIncomingConnect(std::shared_ptr<Connection> connection);
            virtual void onDisconnect(std::shared_ptr<Connection> connection);
            virtual void onMessage(std::shared_ptr<Connection> connection, Message& msg);

        private:
            // Called by beginMining
            void mineBlock(std::string rewardAddress);

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

            // Members
            Mempool       _mempool;
            StorageEngine _storageEngine;
            WalletManager _walletManager;

            std::thread _miningThr;
            bool        _miningStatus = false;
    };
}

#endif