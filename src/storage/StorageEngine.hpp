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
    // Forward declare
    class Node;

    class StorageEngine
    {
        public:
            StorageEngine() = default;

            void loadChain();
            void loadWallets();
            void loadPeers();

        private:

    };
}

#endif