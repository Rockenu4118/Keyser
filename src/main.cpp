//
//  Keyser Blockchain Protocol
//
//  Created by AJ Hermosillo on July 13, 2023.
//  Copyright (c) 2015 2023 Hermosillo. All rights reserved.
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
    std::cout << "Program running...\n" << std::endl;

    std::string customKey = "38B15E3C2210827DEB42C39FBC04D2D8268B5F7B7F1DC2DA75FD620BBD2F4E01";

    Keyser::Wallet ajWallet = Keyser::Wallet("AJ", customKey);
    std::cout << ajWallet << std::endl;

    // Initialize chain
    // std::cout << "\nInitializing chain...\n" << std::endl;
    // Keyser::Chain chain = Keyser::Chain(4, 100);
    // std::cout << *chain.getCurrBlock() << std::endl;

    // Keyser::Transaction tx = Keyser::Transaction(100, "AJ", "none");
    // tx.signTransaction(ajWallet.getKeyPair());
    // chain.createTransaction(tx);

    // chain.mineBlock("AJ");
    // std::cout << *chain.getCurrBlock() << std::endl;

    // chain.mineBlock("AJ");
    // std::cout << *chain.getCurrBlock() << std::endl;


    // chain.getAddressBalance("AJ");
    

    return 0;
}