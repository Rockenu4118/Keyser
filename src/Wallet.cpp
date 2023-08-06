#include <iostream>
#include <string>
#include <openssl/evp.h>
#include <openssl/ec.h>

#include "./include/Wallet.hpp"
#include "./include/ECKeyPair.hpp"
#include "./include/cryptography.hpp"


// Constructors
Keyser::Wallet::Wallet()
{    
    // Generate EC Key object
    _keyPair = ECKeyPair();

    // Calculate public address with previously generated EC uncompressed public key
    calcAddress();
}

// Accessors
std::string Keyser::Wallet::getPublicAddress()
{
    return _publicAddress;
}

// Modifiers
void Keyser::Wallet::calcAddress()
{   
    // Step 1: Convert the raw hex characters of the uncompressed public key to a string.
    std::string unhashed = cryptography::hexToString(_keyPair.getUPublicKey());
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