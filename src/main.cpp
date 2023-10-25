//
//  keyser Blockchain Protocol
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

#include "./net/include/Server.hpp"
#include "./net/include/Client.hpp"
#include "./chain/include/Chain.hpp"
#include "./net/include/MsgTypes.hpp"
#include "./views/cli/include/Prompt.hpp"
#include "./chain/include/Wallet.hpp"
#include "./chain/include/WalletCollection.hpp"
#include "./node/include/Node.hpp"
#include "./data/include/keys.hpp"

int main()
{
    keyser::WalletCollection wallets;

    keyser::Wallet ajWallet("AJ", key1);
    wallets.addWallet(ajWallet);

    keyser::Wallet guyWallet("Guy", key2);
    wallets.addWallet(guyWallet);

    char nodeType;
    char selection;
    bool miningStatus = false;

    
    std::cout << "Select Node type:" << std::endl;
    std::cout << "[1] Full Node"     << std::endl;
    std::cout << "[2] Wallet Node"   << std::endl;
    std::cout << "[0] Exit"          << std::endl;
    std::cout << std::endl;
    std::cin  >> nodeType;
    

    keyser::Node* node;
    
    switch (nodeType)
    {
        case '1':
            node = new keyser::Node(keyser::Node::NodeType::FullNode, 6000);
            node->start();
            break;
        case '2':
            node = new keyser::Node(keyser::Node::NodeType::WalletNode, 6000);
            node->start();
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
                keyser::cli::promptMiningMenu(node, miningStatus);
                break;
            case '2':
                keyser::cli::promptTransactionMenu(node);
                break;
            case '3':
                keyser::cli::promptWalletMenu(wallets);
                break;
            case '4':
                node->ping();
                break;
            default:
                std::cout << "Exiting program..." << std::endl;
                break;
        }
    }
    while (selection != '0');

    // if (selection == 1) {
    //     Server server(6000);
    //     server.start();

    //     while (1) {
    //         server.update();
    //     }
    // } else {
    //     Client client = Client();
    //     client.connect("127.0.0.1", 6000);

    //     int other = 1;
    //     while (other) {
    //         if (!client.incoming().empty())
    //         {
    //             auto msg = client.incoming().popFront();
    //             if (msg._msg.header.id == MsgTypes::ServerAccept)
    //             {
    //                 std::cout << "Server Accepted" << std::endl;
    //             } else {
    //                 std::cout << "Printing message: ";
    //                 msg._msg.printMsg();
    //                 std::cout << std::endl;
    //             }
    //         }
    //         std::cout << "Ping-1, MsgAll-2, Quit-0: ";
    //         std::cin >> other;
    //         if (other == 1)
    //         {
    //             client.ping();
    //         } else {
    //             client.messageAll();
    //         }
    //     }
    // }

   

    

    

    return 0;
}