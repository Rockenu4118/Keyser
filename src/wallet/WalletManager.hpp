#ifndef WALLET_COLLECTION_H
#define WALLET_COLLECTION_H

#include <string>
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
            void getWallet(Wallet& wallet, uint index);
            void displayWallets();

        private:
            std::vector<Wallet> _wallets;
    };
}

#endif