#include <iostream>
#include <string>
#include <openssl/evp.h>
#include <openssl/ec.h>

#include "./include/Wallet.hpp"
#include "./include/ECKeyPair.hpp"
#include "./include/cryptography.hpp"


// Constructors
Keyser::Wallet::Wallet(std::string owner)
{    
    _owner = owner;

    // Generate EC Key Pair object
    _keyPair = new ECKeyPair();

    // Calculate public address with previously generated EC uncompressed public key
    _publicAddress = cryptography::pubKeytoAddress(_keyPair->getUPublicKey());
}

Keyser::Wallet::Wallet(std::string owner, std::string privateKey)
{
    _owner = owner;

    // Insert provided private key
    _keyPair = new ECKeyPair(privateKey);

    // Calculate public address with previously provided private key
    _publicAddress = cryptography::pubKeytoAddress(_keyPair->getUPublicKey());
}

// Accessors
std::string Keyser::Wallet::getPublicAddress()
{
    return _publicAddress;
}

std::string Keyser::Wallet::getOwner()
{
    return _owner;
}

Keyser::ECKeyPair* Keyser::Wallet::getKeyPair()
{
    return _keyPair;
}

// IO Stream operators
namespace Keyser
{
    std::ostream& operator<<(std::ostream& out, Wallet& data) {
        out << "Owner: "          << data.getOwner() << ", ";
        out << "Public address: " << data.getPublicAddress();

        return out;
    }
}