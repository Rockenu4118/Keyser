//
//  Keyser Blockchain Protocol
//
//  Created by AJ Hermosillo on July 13, 2023.
//  Copyright (c) 2015 2023 Hermosillo. All rights reserved.
//

#include <iostream>
#include <string>
#include <vector>
#include <set>

#include "./wallet/WalletManager.hpp"
#include "./views/RootView.hpp"
#include "./node/Node.hpp"
#include "./data/keys.hpp"

#include "./node/NodeInfo.hpp"




int main()
{   
    // std::set<keyser::NodeInfo> info;

    // keyser::NodeInfo info1;
    // info1._address = "127.0.0.1";
    // info1._port = 6000;


    // keyser::NodeInfo info2;
    // info2._address = "127.0.0.1";
    // info2._port = 6001;

    // keyser::NodeInfo info3;
    // info3._address = "127.0.0.1";
    // info3._port = 6000;

    // keyser::NodeInfo info4;
    // info4._address = "127.0.0.1";
    // info4._port = 6001;

    // keyser::NodeInfo info5;
    // info5._address = "192.168.2.22";
    // info5._port = 6001;

    // info.insert(info1);
    // info.insert(info2);
    // info.insert(info3);
    // info.insert(info4);

    // for (auto& node : info)
    //     std::cout << node._address << ":" << node._port << std::endl;

    // std::cout << info.count(info4) << std::endl;



    // Initialize wallet manager and test wallets
    keyser::WalletManager wallets;

    keyser::Wallet ajWallet("AJ", key1);
    wallets.addWallet(ajWallet);

    keyser::Wallet guyWallet("Guy", key2);
    wallets.addWallet(guyWallet);

    // Prompt for port to run on
    int  serverPort;
    char selection;
    bool falseVal = false;

    std::cout << "Server port: ";
    std::cin  >> serverPort;
    
    // Startup node
    keyser::Node* node;
    node = new keyser::Node(serverPort);
    node->start();

    // Initialize CLI
    keyser::cli::RootView view(node, wallets);

    return 0;
}