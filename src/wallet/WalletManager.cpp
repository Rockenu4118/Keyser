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

const keyser::Wallet& keyser::WalletManager::at(int index)
{
    return _wallets.at(index);
}

nlohmann::json keyser::WalletManager::json() const
{
    nlohmann::json json = nlohmann::json::array();

    for (auto& wallet : _wallets)
        json.push_back(wallet.json());

    return json;
}

void keyser::WalletManager::json(nlohmann::json json)
{
    for (auto walletJson : json)
    {
        Wallet wallet(walletJson);
        _wallets.push_back(wallet);
    }
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
    }
}

size_t keyser::WalletManager::count()
{
    return _wallets.size();
}