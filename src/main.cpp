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
#include "./net/include/Server.hpp"
#include "./net/include/Client.hpp"
#include "./chain/include/Chain.hpp"
#include "./net/include/MsgTypes.hpp"
#include "./views/cli/include/Prompt.hpp"
#include "./chain/include/Wallet.hpp"
#include "./chain/include/WalletCollection.hpp"

int main()
{
    std::string key1 = "38B15E3C2210827DEB42C39FBC04D2D8268B5F7B7F1DC2DA75FD620BBD2F4E01";
    std::string key2 = "18E14A7B6A307F426A94F8114701E7C8E774E7F9A47E2C2035DB29A206321725";

    Keyser::WalletCollection wallets;

    Keyser::Wallet ajWallet("AJ", key1);
    wallets.addWallet(ajWallet);

    Keyser::Wallet guyWallet("Guy", key2);
    wallets.addWallet(guyWallet);

    // int serverPort;
    // int clientPort;
    int  selection;
    bool miningStatus = false;


    // Keyser::cli::promptNetConfig(serverPort, clientPort);
    // // Keyser::cli::promptChainConfig(selection, difficulty, reward);

    // Server server(serverPort);
    // server.start();

    // Client client;
    // client.connect("127.0.0.1", clientPort);

    Keyser::Chain chain(5, 100);

    do
    {
        
        Keyser::cli::promptMainMenu(selection, miningStatus);

        switch (selection)
        {
            case 1:
                std::cout << "Begun mining." << std::endl;
                break;
            case 2:
                Keyser::cli::promptTransactionMenu();
                break;
            case 3:
                Keyser::cli::promptWalletMenu(wallets);
                break;
            default:
                std::cout << "Exiting program..." << std::endl;
                break;
        }
    } 
    while (selection != 0);
    
    





    // sleep(30);

    // Begin chain initialization and sequence
    // InitChain chain = InitChain();
    // chain.initChain();


    // int selection;
    // std::cout << "(1 - Server / 2 - Client): ";
    // std::cin >> selection;

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