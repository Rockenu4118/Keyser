//
//  Keyser Blockchain Protocol
//
//  Created by AJ Hermosillo on July 13, 2023.
//  Copyright (c) 2023 Hermosillo. All rights reserved.
//

// #include "./app/App.hpp"
// #include "./chain/Block.hpp"
// #include "./chain/Transaction.hpp"
// #include "./storage/StorageEngine.hpp"
// #include "./wallet/Account.hpp"

#include <keycrypto/Stealth.hpp>
#include <keycrypto/aes.hpp> 
#include <keycrypto/Ring.hpp>
#include <keycrypto/utils.hpp>
#include <keycrypto/sha.hpp>
#include <tests/ring.cpp>
#include <tests/pcommitment.cpp>
#include <string>
#include <map>

#include <openssl/aes.h>
#include <openssl/evp.h>

using namespace std;



int main()
{   
    std::cout << "Running..." << std::endl;

    test();
    // commitment_test();
    





    // keyser::App app;
    // app.run();

    return 0;
}