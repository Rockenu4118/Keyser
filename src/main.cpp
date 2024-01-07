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