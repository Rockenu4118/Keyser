//
//  Keyser Blockchain Protocol
//
//  Created by AJ Hermosillo on July 13, 2023.
//  Copyright (c) 2023 Hermosillo. All rights reserved.
//

#include <iostream>
#include <string>
#include <vector>

#include "./views/RootView.hpp"
#include "./node/Node.hpp"


int main()
{   
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
    keyser::cli::RootView view(node);

    return 0;
}