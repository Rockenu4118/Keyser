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

// Example wallets below...
// Owner: AJ,  Public address: 0xc6d8a2c830495d07318212e9f2cad16f
// Owner: Guy, Public address: 0x183944191006324a447bdb2d98d4b408

int main()
{
    // Begin program and print initialized wallets
    std::cout << "Program running...\n" << std::endl;

    std::string key1 = "38B15E3C2210827DEB42C39FBC04D2D8268B5F7B7F1DC2DA75FD620BBD2F4E01";
    std::string key2 = "18E14A7B6A307F426A94F8114701E7C8E774E7F9A47E2C2035DB29A206321725";

    Keyser::Wallet ajWallet = Keyser::Wallet("AJ", key1);
    std::cout << ajWallet << std::endl;

    Keyser::Wallet guyWallet = Keyser::Wallet("Guy", key2);
    std::cout << guyWallet << std::endl;

    // Initialize chain
    std::cout << "\nInitializing chain...\n" << std::endl;
    Keyser::Chain chain = Keyser::Chain(1, 100);
    std::cout << *chain.getCurrBlock() << std::endl;

    Keyser::Transaction tx = Keyser::Transaction(100, "0xc6d8a2c830495d07318212e9f2cad16f", guyWallet.getKeyPair()->getUPublicKey());
    tx.sign(guyWallet.getKeyPair());
    tx.isValid();
    chain.createTransaction(tx);

    chain.mineBlock("0xc6d8a2c830495d07318212e9f2cad16f");
    // std::cout << *chain.getCurrBlock() << std::endl;

    chain.mineBlock("0xc6d8a2c830495d07318212e9f2cad16f");
    // std::cout << *chain.getCurrBlock() << std::endl;


    chain.getAddressBalance("0xc6d8a2c830495d07318212e9f2cad16f");
    

    return 0;
}