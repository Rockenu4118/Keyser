#ifndef AES_H
#define AES_H

#include <iostream>
#include <string>
// #include <openssl/conf.h>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <openssl/evp.h>
#include <openssl/kdf.h>
// #include <openssl/err.h>

#include "./utils.hpp"

#define KEY_LEN 32
#define IV_LEN 16

#define SCRYPT_N 1048576
#define SCRYPT_r 8
#define SCRYPT_p 1


namespace crypto
{
    namespace sym
    {
        class AESCipher
        {
            public:
                AESCipher();

                AESCipher(std::string password);

                ~AESCipher() = default;

                int encrypt(const std::string& plainText, unsigned char *ciphertext);

                int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *plaintext);

                // inline std::string getKey() { return _key; }

                // inline std::string getIV() { return _iv; }

            private:
                std::string _password;

                unsigned char _key[KEY_LEN];
                unsigned char _iv[IV_LEN];
        };
    }   
}




#endif