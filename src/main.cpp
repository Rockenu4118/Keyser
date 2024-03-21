//
//  Keyser Blockchain Protocol
//
//  Created by AJ Hermosillo on July 13, 2023.
//  Copyright (c) 2023 Hermosillo. All rights reserved.
//

#include <iostream>
#include <string>
#include <unordered_map>

#include "./views/RootView.hpp"
#include "./node/Node.hpp"
#include "./rpc/rpc.hpp"



int main()
{   
    // Prompt for port to run on
    int  port;
    char selection;

    std::cout << "Port: ";
    std::cin  >> port;
    
    // Startup node
    keyser::Node* node;
    node = new keyser::Node(port);
    node->run();

    // Startup RPC API
    keyser::RPC rpc(node);
    rpc.run();

    // Initialize CLI
    keyser::cli::RootView view(node);

    return 0;
}