#include <iostream>
#include <string>
#include <openssl/evp.h>

#include "./include/Wallet.hpp"
#include "./include/cryptography.hpp"


// Constructors
Keyser::Wallet::Wallet()
{    
    // Generate EC keys and assign them to the corresponding member variables.
    cryptography::ecKeys(_privateKey, _uPublicKey, _cPublicKey);

    // Calculate the public address with the previously generated uncompressed public key.
    calcAddress();
}

// Accessors
std::string Keyser::Wallet::getPublicAddress()
{
    return _publicAddress;
}

std::string Keyser::Wallet::getPrivateKey()
{
    return _privateKey;
}

std::string Keyser::Wallet::getUPublicKey()
{
    return _uPublicKey;
}

std::string Keyser::Wallet::getCPublicKey()
{
    return _cPublicKey;
}

// Modifiers
void Keyser::Wallet::calcAddress()
{   
    // Step 1: Convert the raw hex characters of the uncompressed public key to a string.
    std::string unhashed = cryptography::hexToString(_uPublicKey);
    std::string hashed = "";

    // Step 2: Generate a sha256 hash of the uncompressed public key.
    cryptography::sha256(unhashed, hashed);

    // Step 3: Extract last 32 (hex) characters, 16 bytes, of the sha256 hash.
    std::string address = hashed.substr(32, 32);

    // Step 4: Prepend "0x" to indicate that the address is in hex format.
    _publicAddress = "0x" + address;
}

// Other
void Keyser::Wallet::printAddress()
{
    std::cout << "Public address: " << _publicAddress << std::endl;
}

void Keyser::Wallet::printKeys()
{
    std::cout << "Private key: " << _privateKey    << std::endl;
    std::cout << "UPublic key: " << _uPublicKey    << std::endl;
    std::cout << "CPublic key: " << _cPublicKey    << std::endl;
}