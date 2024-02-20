#include <iostream>
#include <string>

#include "./WalletManager.hpp"
#include "./Wallet.hpp"


void keyser::WalletManager::addWallet(Wallet wallet)
{
    _wallets.push_back(wallet);
}

void keyser::WalletManager::createWallet(std::string name)
{
    Wallet newWallet(name);

    _wallets.push_back(newWallet);
}

keyser::Wallet keyser::WalletManager::at(int index)
{
    return _wallets.at(index);
}

void keyser::WalletManager::displayWallets()
{
    if (count() == 0)
    {
        std::cout << "No wallets." << std::endl;
        return;
    }

    for (int i = 0 ; i < count() ; i++)
    {
        std::cout << "#" << i << ", " << _wallets.at(i) << std::endl;
        std::cout << *_wallets.at(i).getKeyPair() << std::endl;
    }
}

size_t keyser::WalletManager::count()
{
    return _wallets.size();
}