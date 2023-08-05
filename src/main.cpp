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
    Keyser::Chain chain = Keyser::Chain(4, 100);
    std::cout << "Chain initialized." << std::endl;

    Keyser::Wallet wallet = Keyser::Wallet();
    // wallet.printAddress();
    
    // chain.createGenesisBlock();

    // chain.mineBlock("theguy");
    // chain.createTransaction(Keyser::Transaction(100, "aj", "theguy"));
    // chain.mineBlock("theguy");
    // chain.getAddressBalance("theguy");

    

    return 0;
}