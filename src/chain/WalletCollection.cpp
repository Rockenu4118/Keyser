#include <iostream>

#include "./include/WalletCollection.hpp"
#include "./include/Wallet.hpp"


void Keyser::WalletCollection::addWallet(Wallet wallet)
{
    wallets.push_back(wallet);
}

void Keyser::WalletCollection::displayWallets()
{
    for (int i = 0 ; i < wallets.size() ; i++)
    {
        std::cout << wallets.at(i) << std::endl;
    }
}