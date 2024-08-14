#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <stdio.h>
#include <string.h>

#include <openssl/evp.h>
#include <openssl/ec.h>
#include <openssl/pem.h>

#include "./crypto.hpp"


// Hashing





// cryptography::ED25519KeyPair::ED25519KeyPair()
// {
//     // genKeyPairObj();
//     // extractKeyPair();
// }

// cryptography::ED25519KeyPair::ED25519KeyPair(cryptography::KeyType type, std::string key)
// {
//     switch (type)
//     {
//         case KeyType::Private:
//             insertPrivateKey(key);
//             extractKeyPair();
//             break;
//         case KeyType::Public:

//             break;
//         default:
//             break;
//     }
// }

// bool cryptography::ED25519KeyPair::genKeyPairObj()
// {
//     EVP_PKEY *my_pkey = nullptr;
//     EVP_PKEY_CTX *my_ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_ED25519, nullptr);

//     if (!EVP_PKEY_keygen_init(my_ctx))
//         return false;

//     if (!EVP_PKEY_keygen(my_ctx, &my_pkey))
//         return false;

//     _keyPairObj = my_pkey;

//     EVP_PKEY_CTX_free(my_ctx);

//     // EVP_PKEY_CTX_keygen1

//     return true;
// }

// bool cryptography::ED25519KeyPair::extractKeyPair()
// {
//     if (_keyPairObj == nullptr)
//     {
//         std::cout << "Nun to extract from" << std::endl;
//         return false;
//     }

//     // std::size_t pub_key_allocated_length;
//     // EVP_PKEY_get_raw_public_key(_keyPairObj, NULL, &pub_key_allocated_length);

//     // unsigned char *public_key = new unsigned char[pub_key_allocated_length];
//     // EVP_PKEY_get_raw_public_key(_keyPairObj, public_key, &pub_key_allocated_length);

//     std::size_t pkey_allocated_length;
//     EVP_PKEY_get_raw_private_key(_keyPairObj, NULL, &pkey_allocated_length);

//     unsigned char *private_key = new unsigned char[pkey_allocated_length];
//     EVP_PKEY_get_raw_private_key(_keyPairObj, private_key, &pkey_allocated_length);

//     std::stringstream privSS;
//     // std::stringstream pubSS;

//     for(unsigned int i = 0; i < 32; ++i)
//     {
//         privSS << std::hex << std::setw(2) << std::setfill('0') << (int)private_key[i];
//         // pubSS  << std::hex << std::setw(2) << std::setfill('0') << (int)public_key[i];
//     }

//     _privateKey = privSS.str();
//     // _publicKey  = pubSS.str();

//     return true;
// }

// bool cryptography::ED25519KeyPair::insertPrivateKey(std::string privateKey)
// {
    // unsigned char* pKey = new unsigned char[32];

    // crypto::utils::parse_string(pKey, privateKey);

    // _keyPairObj = EVP_PKEY_new_raw_private_key(NID_ED25519, nullptr, pKey, 32);

    // BN_bn2hex

    // BIGNUM* r = BN_new();
    
    // BN_rand(r, 256, BN_RAND_TOP_ANY, BN_RAND_BOTTOM_ANY);

    // BN_rand_ex
    // char* rStr = BN_bn2hex(r);
    // char* rStr = BN_bn2dec(r);

    // BN_generate_prime_ex()

    // std::cout << rStr << std::endl;

    // DH_com

    // EVP_PKEY_derive();

    // GROUP_new


    // BN_hex2bn(&privKey, privateKey.c_str());


    // EC_POINT_mul(ed25519_group, pubKey, privKey, nullptr, nullptr, nullptr);
    

    // // Compute public key from previously inserted private key
    // EC_POINT_mul(secp256k1_group, pubKey, privKey, NULL, NULL, NULL);

    // // Insert public key into EC KEY object
    // EC_KEY_set_public_key(keyPair, pubKey);
// }

// Constructors


// IO Stream operators
// namespace cryptography
// {
//     std::ostream& operator<<(std::ostream& out, ED25519KeyPair& data)
//     {
//         out << "Private Key: " << data.getPrivateKey() << "\n";
//         out << "Public Key:  " << data.getPublicKey();

//         return out;
//     }
// }