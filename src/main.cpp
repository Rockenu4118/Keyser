//
//  Keyser Blockchain Protocol
//
//  Created by AJ Hermosillo on July 13, 2023.
//  Copyright (c) 2015 AJ Hermosillo. All rights reserved.
//

#include <iostream>
#include <string>
#include <ctime>

#include "./include/Chain.hpp"
#include "./include/Transaction.hpp"
#include "./include/Wallet.hpp"
#include "./include/util.hpp"

int main()
{
    // Begin program and print initialized wallets
    std::cout << "Program running, wallets shown below.\n" << std::endl;

    Keyser::Wallet ajWallet = Keyser::Wallet("AJ");
    std::cout << ajWallet << std::endl;
    Keyser::Wallet himWallet = Keyser::Wallet("Him");
    std::cout << himWallet << std::endl;

    // Initialize chain
    std::cout << "\nInitializing chain...\n" << std::endl;
    Keyser::Chain chain = Keyser::Chain(4, 100);
    std::cout << *chain.getCurrBlock() << std::endl;

    chain.mineBlock("AJ");
    std::cout << *chain.getCurrBlock() << std::endl;

    Keyser::Transaction tx = Keyser::Transaction(100, "AJ", "none");
    chain.createTransaction(tx);

    chain.mineBlock("AJ");
    std::cout << *chain.getCurrBlock() << std::endl;


    // chain.getAddressBalance("AJ");
    

    return 0;
}