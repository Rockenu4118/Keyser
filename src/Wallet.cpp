#include <iostream>
#include <string>
#include <openssl/evp.h>
#include <openssl/ec.h>

#include "./include/Wallet.hpp"
#include "./include/cryptography.hpp"


// Constructors
Keyser::Wallet::Wallet()
{    
    // Generate EC Key object
    genKeyPairObj();

    // Extract private and public keys from EC key object
    extractKeys();

    // Calculate the public address with the previously extracted uncompressed public key
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
bool Keyser::Wallet::genKeyPairObj()
{
    EC_KEY* keyPair = nullptr;
    
    int success;

    // Generate secp256k1 key pair
    keyPair = EC_KEY_new_by_curve_name(NID_secp256k1);
    success = EC_KEY_generate_key(keyPair);
    if (!success) { 
        std::cout << "Error generating keys." << std::endl;
        return false;
    }

    _keyPairObj = keyPair;
}

bool Keyser::Wallet::extractKeys()
{
    // Check to see if wallet object contains a generated EC Key object
    if (_keyPairObj == NULL) { return false; }

    const BIGNUM*   privKey;
    const EC_POINT* pubKey;
    EC_GROUP*       secp256k1_group;

    // Private key, uncompressed and compressed public keys
    char* privKeyChar;
    char* uPubKeyChar;
    char* cPubKeyChar;

    // Extract private key
    privKey     = EC_KEY_get0_private_key(_keyPairObj);
    privKeyChar = BN_bn2hex(privKey);

    // Extract public key
    pubKey          = EC_KEY_get0_public_key(_keyPairObj);
    secp256k1_group = EC_GROUP_new_by_curve_name(NID_secp256k1);
    uPubKeyChar     = EC_POINT_point2hex(secp256k1_group, pubKey, POINT_CONVERSION_UNCOMPRESSED, nullptr);
    cPubKeyChar     = EC_POINT_point2hex(secp256k1_group, pubKey, POINT_CONVERSION_COMPRESSED, nullptr);

    // Free memory
    EC_GROUP_free(secp256k1_group);

    // Assign extracted keys to corresponding member variables
    _privateKey = privKeyChar;
    _uPublicKey = uPubKeyChar;
    _cPublicKey = cPubKeyChar;

    return true;
}

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