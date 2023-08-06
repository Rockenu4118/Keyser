#include <iostream>
#include <string>

#include <openssl/ec.h>
#include <openssl/evp.h>

#include "./include/ECKeyPair.hpp"


// Constructors
Keyser::ECKeyPair::ECKeyPair()
{
    genKeyPairObj();
    extractKeys();
}

// Accessors
std::string Keyser::ECKeyPair::getPrivateKey()
{
    return _privateKey;
}

std::string Keyser::ECKeyPair::getUPublicKey()
{
    return _uPublicKey;
}

std::string Keyser::ECKeyPair::getCPublicKey()
{
    return _cPublicKey;
}

// Modifiers
bool Keyser::ECKeyPair::genKeyPairObj()
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

    return true;
}

bool Keyser::ECKeyPair::extractKeys()
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

// Other
void Keyser::ECKeyPair::printKeys()
{
    std::cout << "Private key: " << _privateKey << std::endl;
    std::cout << "UPublic key: " << _uPublicKey << std::endl;
    std::cout << "CPublic key: " << _cPublicKey << std::endl;
}