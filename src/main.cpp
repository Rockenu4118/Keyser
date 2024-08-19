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
#include <aes.hpp>
#include <string>
#include <map>

#include <openssl/aes.h>
#include <openssl/evp.h>

using namespace std;



std::string addrGen(std::string pubSpend, std::string pubView)
{
    return pubSpend + pubView;
}

int main()
{   
    std::cout << "Running..." << std::endl;

    crypto::sym::AESCipher cipher;

    std::string msg = "This is a really long image, oh im sorry, a text to demonstarte symmetric encryption using the AES algo!";

    unsigned char ciphertext[256];
    unsigned char decryptedtext[256];

    int decryptedtext_len, ciphertext_len;

    ciphertext_len = cipher.encrypt(msg, ciphertext);

    decryptedtext_len = cipher.decrypt(ciphertext, ciphertext_len, decryptedtext);

    decryptedtext[decryptedtext_len] = '\0';

    std::string text = (char*)decryptedtext;

    std::cout << "Str: " << text << std::endl;

    





    // keyser::App app;
    // app.run();

    return 0;
}