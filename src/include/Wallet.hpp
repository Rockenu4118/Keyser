#ifndef WALLET_H
#define WALLET_H

#include <string>


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

            // Other
            void printAddress();
            void printKeys();

        private:
            std::string _publicAddress;
            std::string _privateKey;
            std::string _uPublicKey;
            std::string _cPublicKey;
    };
}



#endif