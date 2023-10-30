#ifndef WALLET_COLLECTION_H
#define WALLET_COLLECTION_H

#include <vector>
#include <memory>

#include "./Wallet.hpp"

namespace keyser
{
    class WalletManager
    {
        public:
            WalletManager() = default;

            void addWallet(Wallet wallet);
            void displayWallets();

        private:
            std::vector<Wallet> wallets;
    };
}

#endif