#ifndef WALLET_H
#define WALLET_H

#include <string>
#include <openssl/ec.h>

#include "./ECKeyPair.hpp"


namespace Keyser
{
    class Wallet
    {
        public:
            // Constructors
            Wallet();

            // Accessors
            std::string getPublicAddress();
            
            // Modifiers
            void calcAddress();
            
            // Other
            void printAddress();

        private:
            std::string _publicAddress;
            ECKeyPair   _keyPair = ECKeyPair();
    };
}



#endif