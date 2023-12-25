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
#include <Net_Connection.hpp>
#include <Net_Interface.hpp>
#include <nlohmann/json.hpp>


#include "./net/Server.hpp"
#include "./net/Client.hpp"
#include "./chain/Chain.hpp"
#include "./chain/Block.hpp"
#include "./net/MsgTypes.hpp"
#include "./views/cli/Prompt.hpp"
#include "./wallet/Wallet.hpp"
#include "./wallet/WalletManager.hpp"
#include "./views/cli/TransactionView.hpp"
#include "./views/cli/ChainView.hpp"
#include "./views/cli/WalletView.hpp"
#include "./views/cli/NetworkView.hpp"
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
    char selection;
    bool falseVal = false;

    std::cout << "Server port: ";
    std::cin  >> serverPort;
            
    keyser::Node* node;
    
    node = new keyser::Node(serverPort);
    node->start();

    do
    {
        keyser::cli::promptMainMenu(selection);

        switch (selection)
        {
            case '1':
            {
                keyser::cli::ChainView view = keyser::cli::ChainView(node);
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
            case '5':
            {
                keyser::cli::NetworkView view = keyser::cli::NetworkView(node);
            }
                break;
            case '6':
                node->InitBlockDownload();
                break;
            default:
                std::cout << "Exiting program..." << std::endl;
                break;
        }
    }
    while (selection != '0');

    return 0;
}