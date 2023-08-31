#ifndef WALLET_H
#define WALLET_H

#include <string>
#include <iostream>
#include <openssl/ec.h>
#include <cryptography.hpp>


namespace Keyser
{
    class Wallet
    {
    // IO Stream operators
    friend std::ostream& operator<<(std::ostream& out, Wallet& data);

    public:
        // Constructors
        Wallet(std::string owner);
        Wallet(std::string owner, std::string privateKey);

        // Accessors
        std::string getOwner();
        std::string getPublicAddress();
        cryptography::ECKeyPair*  getKeyPair();

    private:
        std::string               _owner;
        std::string               _publicAddress;
        cryptography::ECKeyPair*  _keyPair;
    };
}



#endif