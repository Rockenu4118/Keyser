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
#include "./include/Server.hpp"
#include "./include/Client.hpp"

int main()
{
    std::cout << "Program running..." << std::endl;

    int selection;
    std::cout << "(1 - Server / 2 - Client): ";
    std::cin >> selection;

    if (selection == 1) {
        Server server(6000);
        server.start();

        while (1) {
            server.update();
        }
    } else {
        Client client = Client();
        client.connect("127.0.0.1", 6000);

        int other = 1;
        while (other) {
            auto msg = client.incoming().popFront()._msg;
            std::cout << "Printing message: " << msg << std::endl;
            std::cout << "Ping-1, MsgAll-2, Quit-0: ";
            std::cin >> other;
            if (other == 1)
            {
                client.ping();
            } else {
                client.messageAll();
            }
        }
        

        // bool quit = false;

        // while (!quit) {
        //     if (client.isConnected()){
                

        //     }
        //     else {
        //         quit = true;
        //     }
        // }
    }

    // Begin chain initialization and sequence
    // InitChain chain = InitChain();
    // chain.initChain();

    

    

    return 0;
}