//
//  Keyser Blockchain Protocol
//
//  Created by AJ Hermosillo on July 13, 2023.
//  Copyright (c) 2015 2023 Hermosillo. All rights reserved.
//

#include <iostream>
#include <string>
#include <ctime>
#include <vector>
#include <unistd.h>
#include <net_message.hpp>
#include <nlohmann/json.hpp>

#include "./net/Server.hpp"
#include "./net/Client.hpp"
#include "./chain/Chain.hpp"
#include "./chain/Block.hpp"
#include "./net/MsgTypes.hpp"
#include "./views/cli/Prompt.hpp"
#include "./chain/Wallet.hpp"
#include "./chain/WalletManager.hpp"
#include "./views/cli/TransactionView.hpp"
#include "./views/cli/ChainView.hpp"
#include "./views/cli/WalletView.hpp"
#include "./node/Node.hpp"
#include "./data/keys.hpp"


int main()
{    
    keyser::WalletManager wallets;

    keyser::Wallet ajWallet("AJ", key1);
    wallets.addWallet(ajWallet);

    keyser::Wallet guyWallet("Guy", key2);
    wallets.addWallet(guyWallet);

    keyser::Transaction tx = keyser::Transaction(100, "0xc6d8a2c830495d07318212e9f2cad16f", ajWallet.getKeyPair()->getUPublicKey());
    tx.sign(ajWallet.getKeyPair());

    keyser::Transaction tx2 = keyser::Transaction(200, "0xc6d8a2c830495d07318212e9f2cad16f", ajWallet.getKeyPair()->getUPublicKey());

    keyser::Transaction tx3 = keyser::Transaction(300, "0xc6d8a2c830495d07318212e9f2cad16f", ajWallet.getKeyPair()->getUPublicKey());


    // std::vector<keyser::Transaction> txs;
    // txs.push_back(tx);
    // txs.push_back(tx2);
    // txs.push_back(tx3);

    // keyser::Block block = keyser::Block(1, 0, "hashalashash", txs);

    // std::cout << (block.hasValidTransactions() ? "Valid: true" : "Valid: false") << std::endl;


    char nodeType;
    int  serverPort;
    int  clientPort;
    char selection;
    bool miningStatus = false;

    
    std::cout << "Select Node type:" << std::endl;
    std::cout << "[1] Full Node"     << std::endl;
    std::cout << "[2] Wallet Node"   << std::endl;
    std::cout << "[0] Exit"          << std::endl;
    std::cout << std::endl;
    std::cout << "Selection: ";
    std::cin  >> nodeType;
    
    if (nodeType == '1')
    {
        std::cout << "Server port: ";
        std::cin  >> serverPort;
        std::cout << "Client port: ";
        std::cin  >> clientPort;
    }

    keyser::node::Node* node;
    
    switch (nodeType)
    {
        case '1':
            node = new keyser::node::Node(keyser::node::Node::NodeType::FullNode, serverPort, miningStatus);
            node->start(clientPort);
            break;
        case '2':
            node = new keyser::node::Node(keyser::node::Node::NodeType::WalletNode, clientPort, miningStatus);
            node->start(clientPort);
            break;
        default:
            std::cout << "Invalid selection..." << std::endl;
            break;
    }

    do
    {
        keyser::cli::promptMainMenu(selection, miningStatus);

        switch (selection)
        {
            case '1':
            {
                keyser::cli::ChainView view = keyser::cli::ChainView(node, miningStatus);
            }
                break;
            case '2':
            {
                keyser::cli::TransactionView view = keyser::cli::TransactionView(wallets, node);
            }
                break;  
            case '3':
            {
                keyser::cli::WalletView view = keyser::cli::WalletView(wallets);
            }   
                break;
            case '4':
            {
                node->ping();
            }
                break;
            default:
                std::cout << "Exiting program..." << std::endl;
                break;
        }
    }
    while (selection != '0');



    return 0;
}