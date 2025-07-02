#include <iostream>

#include "./Wallet.hpp"
#include "../data/keys.hpp"


keyser::Wallet::Wallet(Node* node) : _node(node)
{
    // Test wallets
    importAccount("AJ", privKey1);
    importAccount("Guy", privKey2);
}

void keyser::Wallet::importAccount(std::string name, std::string privKey)
{
    Account account(name, privKey);
    _accounts[name] = account;
}

void keyser::Wallet::createAccount(std::string name)
{
    Account newAcc(name);
    _accounts[name] = newAcc;
}

const keyser::Account& keyser::Wallet::get(std::string name)
{
    return _accounts[name];
}

bool keyser::Wallet::createTransaction(int64_t amount, std::string recipient, Account sender)
{
    // int64_t currFunds = 0;
    // std::vector<UTXO> utxos;

    // for (auto utxo : _node->utxoSet()->possibleUtxos(sender.getPublicAddress()))
    // {
    //     if (currFunds < amount)
    //     {
    //         utxos.push_back(utxo);
    //         currFunds += utxo._output._amount;
    //     }
    //     else
    //         break;
    // }

    // if (currFunds < amount)
    //     return false;

    
    // Transaction tx(utxos, amount, 0, recipient, sender.getKeyPair()->getUPublicKey());
    // tx.sign(sender.getKeyPair());

    // return _node->submitTransaction(tx);
    return true;
}

// nlohmann::json keyser::Wallet::json() const
// {
//     nlohmann::json json = nlohmann::json::array();
//
//     for (auto& account : _accounts)
//         json.push_back(account.second.json());
//
//     return json;
// }
//
// void keyser::Wallet::json(nlohmann::json json)
// {
//     for (auto accountJson : json)
//     {
//         Account account(accountJson);
//         _accounts[account.getName()] = account;
//     }
// }

// void keyser::Wallet::displayAccounts()
// {
//     if (count() == 0)
//     {
//         std::cout << "No accounts." << std::endl;
//         return;
//     }

    // for (auto& account : _accounts)
    // {
    //     // std::cout << account.second << std::endl;
    // }
// }

size_t keyser::Wallet::count() const
{
    return _accounts.size();
}