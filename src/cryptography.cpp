#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <stdio.h>
#include <string.h>

#include <openssl/evp.h>
#include <openssl/ec.h>
#include <openssl/ripemd.h>
#include <openssl/pem.h>

#include "./include/cryptography.hpp"


// Hashing
bool cryptography::sha256(const std::string& unhashed, std::string& hashed)
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

// Asymmetric encryption
bool cryptography::ecKeys(std::string& privateKey, std::string& uPublicKey, std::string& cPublicKey)
{
    EC_KEY*         keyPair = nullptr;
    const BIGNUM*   privKey;
    const EC_POINT* pubKey;
    EC_GROUP*       secp256k1_group;

    char* privKeyChar;

    // Uncompressed and compressed public keys
    char* uPubKeyChar;
    char* cPubKeyChar;
    
    int success;

    // Generate secp256k1 key pair
    keyPair = EC_KEY_new_by_curve_name(NID_secp256k1);
    success   = EC_KEY_generate_key(keyPair);
    if (!success) { 
        std::cout << "Error generating keys." << std::endl;
        return false;
    }

    // Extract private key
    privKey     = EC_KEY_get0_private_key(keyPair);
    privKeyChar = BN_bn2hex(privKey);

    // Extract public key
    pubKey          = EC_KEY_get0_public_key(keyPair);
    secp256k1_group = EC_GROUP_new_by_curve_name(NID_secp256k1);
    uPubKeyChar     = EC_POINT_point2hex(secp256k1_group, pubKey, POINT_CONVERSION_UNCOMPRESSED, nullptr);
    cPubKeyChar     = EC_POINT_point2hex(secp256k1_group, pubKey, POINT_CONVERSION_COMPRESSED, nullptr);

    // Free memory
    EC_GROUP_free(secp256k1_group);

    privateKey = privKeyChar;
    uPublicKey = uPubKeyChar;
    cPublicKey = cPubKeyChar;
    
    return true;
}

// Other
std::string cryptography::hexToString(const std::string& input)
{
    std::string output;
    if ((input.length() % 2) != 0) {
        std::cout << "String is not valid length." << std::endl;
    }
    size_t count = input.length() / 2;
    for (size_t i = 0 ; count > i ; i++) {
        uint32_t s = 0;
        std::stringstream ss;
        ss << std::hex << input.substr(i * 2, 2);
        ss >> s;
        output.push_back(static_cast<unsigned char>(s));
    }
    return output;
}