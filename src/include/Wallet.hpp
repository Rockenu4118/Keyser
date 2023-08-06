#ifndef WALLET_H
#define WALLET_H

#include <string>
#include <openssl/ec.h>


namespace Keyser
{
    class Wallet
    {
        public:
            // Constructors
            Wallet();

            // Accessors
            std::string getPublicAddress();
            std::string getPrivateKey();
            std::string getUPublicKey();
            std::string getCPublicKey();

            // Modifiers
            void calcAddress();
            bool genKeyPairObj();
            bool extractKeys();

            // Other
            void printAddress();
            void printKeys();

        private:
            std::string _publicAddress;
            EC_KEY*     _keyPairObj;
            std::string _privateKey;
            std::string _uPublicKey;
            std::string _cPublicKey;
    };
}



#endif