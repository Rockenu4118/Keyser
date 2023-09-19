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

#include "./include/init.hpp"


enum class MsgTypes : uint32_t
{
    SendVersion,
    SendTransaction
};

int main()
{
    std::cout << "Program running...\n";

    networking::Message<MsgTypes> msg;
    msg.header.id = MsgTypes::SendVersion;
 

    std::cout << msg;


    // Begin chain initialization and sequence
    // InitChain chain = InitChain();
    // chain.initChain();

    // TCP Stuff
    // InitNet net = InitNet();
    // net.initNet();

    

    return 0;
}