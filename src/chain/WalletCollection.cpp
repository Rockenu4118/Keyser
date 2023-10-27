#include <iostream>

#include "./WalletCollection.hpp"
#include "./Wallet.hpp"


void keyser::WalletCollection::addWallet(Wallet wallet)
{
    wallets.push_back(wallet);
}

void keyser::WalletCollection::displayWallets()
{
    for (int i = 0 ; i < wallets.size() ; i++)
    {
        std::cout << wallets.at(i) << std::endl;
    }
}