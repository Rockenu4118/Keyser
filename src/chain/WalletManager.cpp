#include <iostream>

#include "./WalletManager.hpp"
#include "./Wallet.hpp"


void keyser::WalletManager::addWallet(Wallet wallet)
{
    wallets.push_back(wallet);
}

void keyser::WalletManager::displayWallets()
{
    for (int i = 0 ; i < wallets.size() ; i++)
    {
        std::cout << wallets.at(i) << std::endl;
    }
}