#ifndef STORAGE_ENGINE_H
#define STORAGE_ENGINE_H

#include <vector>
#include <memory>
#include <deque>

#include "../chain/Block.hpp"
#include "../wallet/WalletManager.hpp"
#include "../node/NodeInfo.hpp"


namespace keyser
{
    class StorageEngine
    {
        public:
            StorageEngine() = default;

            void loadChain(std::vector<std::shared_ptr<Block>> blocks);
            void loadWallets(WalletManager& walletManager);
            void loadPeers(std::deque<NodeInfo>& potentialConnections);


        private:

    };
}

#endif