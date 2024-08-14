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

#include <Stealth.hpp>
#include <string>
#include <map>

#include <openssl/aes.h>
#include <openssl/evp.h>

#include <crypto++/sha.h>
#include <crypto++/integer.h>
#include <crypto++/eccrypto.h>

using namespace std;
using namespace CryptoPP;

void ahh()
{
    
}


std::string addrGen(std::string pubSpend, std::string pubView)
{
    return pubSpend + pubView;
}

int main()
{   

    std::cout << "Running..." << std::endl;
    crypto::StealthKeys aj;

    // std::cout << aj << std::endl;

    crypto::StealthKeys alice("D1D215CF7D7D790D52BF2C380269A1B64699385B4FE119E7DFD16BCD2BF0DCFD");


    crypto::StealthKeys bob("F14B047E6EBD4C0A371E3DC0478561EA686C327EB9FFE593A35A300D8072A271");

    std::string R;

    std::string bob2aliceAddr = bob.genStealthAddr(alice.getPublicAddr(), R);

    std::string image =  alice.genImage(bob2aliceAddr, R);

    std::cout << "Image: " << image << std::endl;




    // std::cout << bob << std::endl;

    // std::cout << 

    // keyser::App app;
    // app.run();

    return 0;
}