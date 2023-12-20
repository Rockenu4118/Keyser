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

    int  serverPort;
    int  clientPort;
    char selection;
    bool miningStatus = false;

    std::cout << "Server port: ";
    std::cin  >> serverPort;
    std::cout << "Client port: ";
    std::cin  >> clientPort;
            
    keyser::node::Node* node;
    
    node = new keyser::node::Node(serverPort, miningStatus);
    node->start(clientPort);        

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