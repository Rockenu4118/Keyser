//
//  Keyser Blockchain Protocol
//
//  Created by AJ Hermosillo on July 13, 2023.
//  Copyright (c) 2023 Hermosillo. All rights reserved.
//

#include "./app/App.hpp"
#include "./chain/Block.hpp"
#include "./chain/Transaction.hpp"
#include "./storage/StorageEngine.hpp"
#include "./wallet/Account.hpp"

#include <leveldb/db.h>
#include <string>
#include <map>




int main()
{  
    std::cout << "Running..." << std::endl;

    // keyser::UTXO tx1;
    // tx1._output._amount = 1000;
    // keyser::UTXO tx2;
    // tx2._output._amount = 122;

    // std::vector<keyser::UTXO> txs;
    // txs.push_back(tx1);
    // txs.push_back(tx2);


    // keyser::Transaction tx(txs, 1100, 0, "0x183944191006324a447bdb2d98d4b408", "jim");

    // for (auto out : tx._outputs)
    //     std::cout << out << std::endl;

    keyser::App app;
    app.run();

    return 0;
}