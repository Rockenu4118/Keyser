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
#include <Server_Interface.hpp>
#include <net_message.hpp>
#include <Connection.hpp>

#include "./include/init.hpp"
#include "./include/Server.hpp"



int main()
{
    std::cout << "Program running...\n";

    // networking::Message<MsgTypes> msg;
    // msg.header.id = MsgTypes::SendVersion;

    Server server(6000);
    server.start();

    while (1) {
        server.update();
    }
 

    // std::cout << msg;
    // msg.size();

    // Begin chain initialization and sequence
    // InitChain chain = InitChain();
    // chain.initChain();

    // TCP Stuff
    // InitNet net = InitNet();
    // net.initNet();

    

    

    return 0;
}