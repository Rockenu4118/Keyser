#include <iostream>
#include <string>

#include "./WalletManager.hpp"
#include "./Wallet.hpp"


void keyser::WalletManager::addWallet(Wallet wallet)
{
    _wallets.push_back(wallet);
}

void keyser::WalletManager::getWallet(Wallet& wallet, uint index)
{
    if (index >= _wallets.size())
        return;
    
    wallet = _wallets.at(index);
}

void keyser::WalletManager::displayWallets()
{
    for (int i = 0 ; i < _wallets.size() ; i++)
    {
        std::cout << "#" << i << " - " << _wallets.at(i) << std::endl;
    }
}