#ifndef NODE_H
#define NODE_H

#include "../net/NetInterface.hpp"
#include "../net/Peer.hpp"

#include "../chain/Chain.hpp"
#include "../storage/StorageEngine.hpp"
#include "../validation/ValidationEngine.hpp"
#include "../chain/Transaction.hpp"
#include "../chain/Block.hpp"
#include "../node/NodeInfo.hpp"
#include "../wallet/WalletManager.hpp"
#include "../chain/Mempool.hpp"


namespace keyser
{
    // Forward declare
    class ValidationEngine;

    class Node : public Chain, public Mempool, public NetInterface
    {
        public:
            enum class Status : uint32_t
            {
                Offline,
                Syncing,
                Online
            };

            Node(uint16_t port);

            ~Node();

            void run();
            void shutdown();

            Status getStatus() const;
            time_t getUptime() const;

            void beginMining(uint numBlocks = -1);
            void stopMining();

            bool createTransaction(Transaction& transaction);

            void broadcastSelf();

            void completedInitialBlockDownload();

            void ping();
            void pong();

            void version(std::shared_ptr<Peer> peer);
            void verack(std::shared_ptr<Peer> peer);

            void getHeaders();
            void sendHeaders(std::shared_ptr<Peer> peer);

            void getBlocks();
            void sendBlocks(std::shared_ptr<Peer> peer, int blockIndex);

            void getNodeList();
            void nodeInfoStream(std::shared_ptr<Peer> peer);

            void getData();
            void inv(std::shared_ptr<Peer> peer, int startingBlock);
            
            WalletManager& walletManager();

        protected:
            // Event handlers
            virtual bool allowConnect(std::shared_ptr<Peer> peer);
            virtual void onOutgoingConnect(std::shared_ptr<Peer> peer);
            virtual void onIncomingConnect(std::shared_ptr<Peer> peer);
            virtual void onDisconnect(std::shared_ptr<Peer> peer);
            virtual void onMessage(std::shared_ptr<Peer> peer, Message& msg);

        private:
            // Distribution msgs only called by public functions
            void distributeNodeInfo(NodeInfo& nodeInfo);
            void distributeBlock(Block& block);
            void distributeTransaction(Transaction& transaction);

            // Msg Handlers
            void handlePing(std::shared_ptr<Peer> peer, Message& msg);
            void handleDistributeNodeInfo(std::shared_ptr<Peer> peer, Message& msg);
            void handleDistributeBlock(std::shared_ptr<Peer> peer, Message& msg);
            void handleDistributeTransaction(std::shared_ptr<Peer> peer, Message& msg);
            void handleVersion(std::shared_ptr<Peer> peer, Message& msg);
            void handleVerack(std::shared_ptr<Peer> peer, Message& msg);
            void handleGetBlocks(std::shared_ptr<Peer> peer, Message& msg);
            void handleInv(std::shared_ptr<Peer> peer, Message& msg);
            void handleGetData(std::shared_ptr<Peer> peer, Message& msg);
            void handleBlock(std::shared_ptr<Peer> peer, Message& msg);
            void handleGetNodeList(std::shared_ptr<Peer> peer, Message& msg);
            void handleNodeInfo(std::shared_ptr<Peer> peer, Message& msg);
            void handleGetHeaders(std::shared_ptr<Peer> peer, Message& msg);
            void handleHeaders(std::shared_ptr<Peer> peer, Message& msg);

            // Members
            Status _status = Status::Offline;

            StorageEngine     _storageEngine;
            ValidationEngine* _validationEngine = nullptr;
            
            WalletManager _walletManager;

            std::thread _miningThr;
            bool        _miningStatus = false;

            time_t _startTime;

            bool _shutdownNode = false;
    };
}

#endif