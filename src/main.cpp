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


    std::vector<keyser::Transaction> txs;
    txs.push_back(tx);
    txs.push_back(tx2);
    txs.push_back(tx3);

    keyser::Block block = keyser::Block(1, nullptr, "hashalashash", txs);

    

    char nodeType;
    char selection;
    bool miningStatus = false;

    
    std::cout << "Select Node type:" << std::endl;
    std::cout << "[1] Full Node"     << std::endl;
    std::cout << "[2] Wallet Node"   << std::endl;
    std::cout << "[0] Exit"          << std::endl;
    std::cout << std::endl;
    std::cin  >> nodeType;
    

    keyser::node::Node* node;
    
    switch (nodeType)
    {
        case '1':
            node = new keyser::node::Node(keyser::node::Node::NodeType::FullNode, 6000);
            node->start();
            break;
        case '2':
            node = new keyser::node::Node(keyser::node::Node::NodeType::WalletNode, 6000);
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
            {
                keyser::cli::promptMiningMenu(node, miningStatus);
            }
                break;
            case '2':
            {
                keyser::cli::TransactionView view = keyser::cli::TransactionView(wallets, node);
            }
                break;  
            case '3':
                // keyser::cli::promptWalletMenu(wallets);
                node->sendBlock(block);
                break;
            case '4':
                node->ping();
                break;
            case '5':
                node->messageAll();
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