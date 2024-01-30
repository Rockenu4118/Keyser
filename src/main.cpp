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
#include <nlohmann/json.hpp>

#include "./wallet/WalletManager.hpp"
#include "./views/RootView.hpp"
#include "./node/Node.hpp"
#include "./data/keys.hpp"

#include "./node/NodeInfo.hpp"


int main()
{   
    // nlohmann::json doc;
    // nlohmann::json info1;
    // info1["version"] = 1;
    // info1["alias"] = "me niggga";

    // nlohmann::json info2;
    // info2["version"] = 1.2;
    // info2["alias"] = "mf G";

    // doc["hello"].push_back("fefef");
    // doc["hello"].push_back("bob");
    // doc["infos"].push_back(info1);
    // doc["infos"].push_back(info2);

    // std::cout << doc.dump(2) << std::endl;

    // for (auto& element : doc["infos"])
    // {   
    //     std::string str = element["alias"];
    //     std::cout << str << std::endl;
    // }

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