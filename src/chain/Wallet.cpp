#include <iostream>
#include <string>
#include <openssl/evp.h>
#include <openssl/ec.h>

#include "./include/Wallet.hpp"
#include "../utils/include/utils.hpp"
#include <cryptography.hpp>


// Constructors
keyser::Wallet::Wallet(std::string name)
{    
    _name = name;

    // Generate EC Key Pair object
    _keyPair = new cryptography::ECKeyPair();

    // Calculate public address with previously generated EC uncompressed public key
    _publicAddress = keyser::utils::pubKeytoAddress(_keyPair->getUPublicKey());
}

keyser::Wallet::Wallet(std::string name, std::string privateKey)
{
    _name = name;

    // Insert provided private key
    _keyPair = new cryptography::ECKeyPair("private", privateKey);

    // Calculate public address with previously provided private key
    _publicAddress = keyser::utils::pubKeytoAddress(_keyPair->getUPublicKey());
}

// Accessors
std::string keyser::Wallet::getPublicAddress()
{
    return _publicAddress;
}

std::string keyser::Wallet::getName()
{
    return _name;
}

cryptography::ECKeyPair* keyser::Wallet::getKeyPair()
{
    return _keyPair;
}

// IO Stream operators
namespace keyser
{
    std::ostream& operator<<(std::ostream& out, Wallet& data) {
        out << "Owner: "          << data.getName() << ", ";
        out << "Public address: " << data.getPublicAddress();

        return out;
    }
}