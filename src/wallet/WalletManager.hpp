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
            void createWallet(std::string name);
            void displayWallets();

            Wallet at(int index);

            size_t count();

        private:
            std::vector<Wallet> _wallets;
    };
}

#endif