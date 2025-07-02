//
//  Keyser Blockchain Protocol
//
//  Created by AJ Hermosillo on July 13, 2023.
//  Copyright (c) 2023 Hermosillo. All rights reserved.
//

#include "./app/App.hpp"
// #include "./chain/Block.hpp"
// #include "./chain/Transaction.hpp"
// #include "./storage/StorageEngine.hpp"
// #include "./wallet/Account.hpp"

#include <openssl/aes.h>
#include <openssl/evp.h>



int main()
{   
    std::cout << "Running..." << std::endl;

    // ring_test();
    // commitment_test();
    // stealth_test();




    keyser::App app;
    app.run();

    return 0;
}