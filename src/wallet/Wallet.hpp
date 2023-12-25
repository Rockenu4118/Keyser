#ifndef WALLET_H
#define WALLET_H

#include <string>
#include <iostream>
#include <openssl/ec.h>
#include <cryptography.hpp>


namespace keyser
{
    class Wallet
    {
    // IO Stream operators
    friend std::ostream& operator<<(std::ostream& out, Wallet& data);

    public:
        // Constructors
        Wallet();
        Wallet(std::string name);
        Wallet(std::string name, std::string privateKey);

        // Accessors
        std::string              getName();
        std::string              getPublicAddress();
        cryptography::ECKeyPair* getKeyPair();

    private:
        std::string               _name;
        std::string               _publicAddress;
        cryptography::ECKeyPair*  _keyPair;
    };
}



#endif