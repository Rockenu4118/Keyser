//
//  Keyser Blockchain Protocol
//
//  Created by AJ Hermosillo on July 13, 2023.
//  Copyright (c) 2023 Hermosillo. All rights reserved.
//

#include "./app/App.hpp"
#include "./chain/Block.hpp"
#include "./storage/StorageEngine.hpp"
#include "./wallet/Wallet.hpp"
#include "./wallet/WalletManager.hpp"

#include <leveldb/db.h>
#include <string>
#include <map>




int main()
{  
    std::cout << "Running..." << std::endl;
    


    keyser::App app;
    app.run();

    return 0;
}