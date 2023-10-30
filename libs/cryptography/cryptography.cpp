#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <stdio.h>
#include <string.h>

#include <openssl/evp.h>
#include <openssl/ec.h>
#include <openssl/pem.h>

#include "./cryptography.hpp"


// Hashing
bool cryptography::SHA256(const std::string& unhashed, std::string& hashed)
{
    bool success = false;

    EVP_MD_CTX* context = EVP_MD_CTX_new();

    if(context != NULL)
    {
        if(EVP_DigestInit_ex(context, EVP_sha256(), NULL))
        {
            if(EVP_DigestUpdate(context, unhashed.c_str(), unhashed.length()))
            {
                unsigned char hash[EVP_MAX_MD_SIZE];
                unsigned int lengthOfHash = 0;
    
                if(EVP_DigestFinal_ex(context, hash, &lengthOfHash))
                {
                    std::stringstream ss;
                    for(unsigned int i = 0; i < lengthOfHash; ++i)
                    {
                        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
                    }
    
                    hashed = ss.str();
                    success = true;
                }
            }
        }
    
        EVP_MD_CTX_free(context);
    }

    return success;
}

// Constructors
cryptography::ECKeyPair::ECKeyPair()
{
    genKeyPairObj();
    extractKeys();
}

cryptography::ECKeyPair::ECKeyPair(std::string keyType, std::string key)
{
    if (keyType == "private") {
        insertPrivateKey(key);
        extractKeys();
    } else if (keyType == "public") {
        insertPublicKey(key);
    }
}

// Accessors
EC_KEY* cryptography::ECKeyPair::getKeyPairObj()
{
    return _keyPairObj;
}

std::string cryptography::ECKeyPair::getPrivateKey()
{
    return _privateKey;
}

std::string cryptography::ECKeyPair::getUPublicKey()
{
    return _uPublicKey;
}

std::string cryptography::ECKeyPair::getCPublicKey()
{
    return _cPublicKey;
}

// Modifiers
bool cryptography::ECKeyPair::genKeyPairObj()
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

bool cryptography::ECKeyPair::insertPrivateKey(std::string privateKey)
{
    if (privateKey.length() != 64) {
        std::cout << "Invalid private key length." << std::endl;
        return false;
    }

    EC_GROUP* secp256k1_group = EC_GROUP_new_by_curve_name(NID_secp256k1);
    EC_KEY*   keyPair         = EC_KEY_new();
    BIGNUM*   privKey         = BN_new();
    EC_POINT* pubKey          = EC_POINT_new(secp256k1_group);
    
    // Convert private key string into BIGNUM
    BN_hex2bn(&privKey, privateKey.c_str());

    // Insert private key into EC_KEY object
    EC_KEY_set_group(keyPair, secp256k1_group);
    EC_KEY_set_private_key(keyPair, privKey);

    // Compute public key from previously inserted private key
    EC_POINT_mul(secp256k1_group, pubKey, privKey, NULL, NULL, NULL);

    // Insert public key into EC KEY object
    EC_KEY_set_public_key(keyPair, pubKey);

    // Free memory
    EC_GROUP_free(secp256k1_group);

    _keyPairObj = keyPair;

    return true;
}

bool cryptography::ECKeyPair::insertPublicKey(std::string publicKey)
{
    if (publicKey.length() != 130) {
        std::cout << "Invalid public key length." << std::endl;
        return false;
    }

    EC_GROUP* secp256k1_group = EC_GROUP_new_by_curve_name(NID_secp256k1);
    EC_KEY*   keyPair         = EC_KEY_new();
    EC_POINT* pubKey          = EC_POINT_new(secp256k1_group);

    // Convert public key string into EC POINT
    EC_POINT_hex2point(secp256k1_group, publicKey.c_str(), pubKey, nullptr);

    // Insert public key into EC KEY object
    EC_KEY_set_group(keyPair, secp256k1_group);
    EC_KEY_set_public_key(keyPair, pubKey);

    // Free memory
    EC_GROUP_free(secp256k1_group);

    _keyPairObj = keyPair;

    return true;
}

bool cryptography::ECKeyPair::extractKeys()
{
    // Check to see if wallet object contains a generated EC Key object
    if (_keyPairObj == nullptr) {
        std::cout << "No key pair object to extract from." << std::endl;
        return false;
    }

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
void cryptography::ECKeyPair::sign(std::string hash, std::string& rSigVal, std::string& sSigVal)
{
    ECDSA_SIG* signature = ECDSA_do_sign((const unsigned char *)hash.c_str(), strlen(hash.c_str()), _keyPairObj);

    const BIGNUM* pr;
    const BIGNUM* ps;

    char* prChar;
    char* psChar;

    ECDSA_SIG_get0(signature, &pr, &ps);

    prChar = BN_bn2hex(pr);
    psChar = BN_bn2hex(ps);

    rSigVal = prChar;
    sSigVal = psChar;

    // ECDSA_SIG_set0();

    // std::cout << rSigVal << std::endl;
    // std::cout << sSigVal << std::endl;
}

bool cryptography::ECKeyPair::verify(std::string hash, std::string rSigVal, std::string sSigVal)
{
    // return ECDSA_do_verify((const unsigned char *)hash.c_str(), strlen(hash.c_str()), signature, _keyPairObj);
}

// IO Stream operators
namespace cryptography
{
    std::ostream& operator<<(std::ostream& out, ECKeyPair& data) {
        out << "Private key: " << data.getPrivateKey() << std::endl;
        out << "UPublic key: " << data.getUPublicKey() << std::endl;

        return out;
    }
}