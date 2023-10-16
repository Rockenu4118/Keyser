#include <iostream>
#include <string>
#include <openssl/evp.h>
#include <openssl/ec.h>

#include "./include/Wallet.hpp"
#include "../utils/include/utils.hpp"
#include <cryptography.hpp>


// Constructors
Keyser::Wallet::Wallet(std::string name)
{    
    _name = name;

    // Generate EC Key Pair object
    _keyPair = new cryptography::ECKeyPair();

    // Calculate public address with previously generated EC uncompressed public key
    _publicAddress = keyser::utils::pubKeytoAddress(_keyPair->getUPublicKey());
}

Keyser::Wallet::Wallet(std::string name, std::string privateKey)
{
    _name = name;

    // Insert provided private key
    _keyPair = new cryptography::ECKeyPair("private", privateKey);

    // Calculate public address with previously provided private key
    _publicAddress = keyser::utils::pubKeytoAddress(_keyPair->getUPublicKey());
}

// Accessors
std::string Keyser::Wallet::getPublicAddress()
{
    return _publicAddress;
}

std::string Keyser::Wallet::getName()
{
    return _name;
}

cryptography::ECKeyPair* Keyser::Wallet::getKeyPair()
{
    return _keyPair;
}

// IO Stream operators
namespace Keyser
{
    std::ostream& operator<<(std::ostream& out, Wallet& data) {
        out << "Owner: "          << data.getName() << ", ";
        out << "Public address: " << data.getPublicAddress();

        return out;
    }
}