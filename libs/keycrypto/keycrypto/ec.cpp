#include "./ec.hpp"


// crypto::ECKeyPair::ECKeyPair()
// {
//     genKeyPairObj();
//     extractKeys();
// }

// crypto::ECKeyPair::ECKeyPair(nlohmann::json json)
// {
//     insertPrivateKey(json["privateKey"]);
//     extractKeys();
// }

// crypto::ECKeyPair::ECKeyPair(KeyType type, std::string key)
// {
//     switch (type)
//     {
//         case KeyType::Private:
//             insertPrivateKey(key);
//             extractKeys();
//             break;
//         case KeyType::Public:
//             insertPublicKey(key);
//         default:
//             break;
//     }
// }

// Modifiers
bool crypto::ECKeyPair::genKeyPairObj()
{
    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_EC, nullptr);

    if (!ctx) return false;

    if (EVP_PKEY_keygen_init(ctx) <= 0) return false;

    if (EVP_PKEY_CTX_set_ec_paramgen_curve_nid(ctx, NID_secp256k1) <= 0) return false;

    if (EVP_PKEY_keygen(ctx, &mKeyPairObj) <= 0)
    {
        EVP_PKEY_CTX_free(ctx);
        return false;
    }

    EVP_PKEY_CTX_free(ctx);
    return true;
}

// bool crypto::ECKeyPair::insertPrivateKey(std::string privateKey)
// {
//     EC_GROUP* secp256k1_group = EC_GROUP_new_by_curve_name(NID_secp256k1);
//     EC_KEY*   keyPair         = EC_KEY_new();
//     BIGNUM*   privKey         = BN_new();
//     EC_POINT* pubKey          = EC_POINT_new(secp256k1_group);
//
//     // EVP_PKEY_CTX_get
//
//     // std::string A = "0400077972767C1E9DB98B4A0E1A8D055BA186B4868D9AF40679B79E610E2185DECBE8E573A70FC647136370173969D55B293798D9E918200BFE9F35C279C135BE";
//     // EC_POINT* aPoint = EC_POINT_new(secp256k1_group);
//
//     // EC_POINT_hex2point(secp256k1_group, A.c_str(), aPoint, nullptr);
//     // char* orderStr;
//     // char* cofactorStr;
//
//     // const BIGNUM* order = EC_GROUP_get0_order(secp256k1_group);
//     // const BIGNUM* cofactor = EC_GROUP_get0_cofactor(secp256k1_group);
//
//     // orderStr = BN_bn2hex(order);
//     // cofactorStr = BN_bn2hex(cofactor);
//
//     // EC_GROUP_set_generator(secp256k1_group, aPoint, order, cofactor);
//
//     // std::cout << orderStr << std::endl;
//     // std::cout << cofactorStr << std::endl;
//
//     // Convert private key string into BIGNUM
//     BN_hex2bn(&privKey, privateKey.c_str());
//
//     // Insert private key into EC_KEY object
//     EC_KEY_set_group(keyPair, secp256k1_group);
//     EC_KEY_set_private_key(keyPair, privKey);
//
//     // Compute public key from previously inserted private key
//     EC_POINT_mul(secp256k1_group, pubKey, privKey, NULL, NULL, NULL);
//
//     // Insert public key into EC KEY object
//     EC_KEY_set_public_key(keyPair, pubKey);
//
//     // Free memory
//     EC_GROUP_free(secp256k1_group);
//
//     _keyPairObj = keyPair;
//
//     return true;
// }

// bool crypto::ECKeyPair::insertPublicKey(std::string publicKey)
// {
//     if (publicKey.length() != 130) {
//         std::cout << "Invalid public key length." << std::endl;
//         return false;
//     }
//
//     EC_GROUP* secp256k1_group = EC_GROUP_new_by_curve_name(NID_secp256k1);
//     EC_KEY*   keyPair         = EC_KEY_new();
//     EC_POINT* pubKey          = EC_POINT_new(secp256k1_group);
//
//     // Convert public key string into EC POINT
//     EC_POINT_hex2point(secp256k1_group, publicKey.c_str(), pubKey, nullptr);
//
//     // Insert public key into EC KEY object
//     EC_KEY_set_group(keyPair, secp256k1_group);
//     EC_KEY_set_public_key(keyPair, pubKey);
//
//     // Free memory
//     EC_GROUP_free(secp256k1_group);
//
//     _keyPairObj = keyPair;
//
//     return true;
// }

// bool crypto::ECKeyPair::extractKeys()
// {
//     // Check to see if wallet object contains a generated EC Key object
//     if (_keyPairObj == nullptr) {
//         std::cout << "No key pair object to extract from." << std::endl;
//         return false;
//     }
//
//     const BIGNUM*   privKey;
//     const EC_POINT* pubKey;
//     EC_GROUP*       secp256k1_group;
//
//     // Private key, uncompressed and compressed public keys
//     char* privKeyChar;
//     char* uPubKeyChar;
//     char* cPubKeyChar;
//
//     // Extract private key
//     privKey     = EC_KEY_get0_private_key(_keyPairObj);
//     privKeyChar = BN_bn2hex(privKey);
//
//     // Extract public key
//     pubKey          = EC_KEY_get0_public_key(_keyPairObj);
//     secp256k1_group = EC_GROUP_new_by_curve_name(NID_secp256k1);
//     uPubKeyChar     = EC_POINT_point2hex(secp256k1_group, pubKey, POINT_CONVERSION_UNCOMPRESSED, nullptr);
//     cPubKeyChar     = EC_POINT_point2hex(secp256k1_group, pubKey, POINT_CONVERSION_COMPRESSED, nullptr);
//
//     // Free memory
//     EC_GROUP_free(secp256k1_group);
//
//     // Assign extracted keys to corresponding member variables
//     _privateKey = privKeyChar;
//     _uPublicKey = uPubKeyChar;
//     _cPublicKey = cPubKeyChar;
//
//     return true;
// }

// nlohmann::json crypto::ECKeyPair::json() const
// {
//     nlohmann::json json;
//
//     json["privateKey"] = _privateKey;
//     json["uPublicKey"] = _uPublicKey;
//     json["cPublicKey"] = _cPublicKey;
//
//     return json;
// }

// Other
// void crypto::ECKeyPair::sign(std::string hash, std::string& rSigVal, std::string& sSigVal)
// {
//     ECDSA_SIG* signature = ECDSA_do_sign((const unsigned char *)hash.c_str(), strlen(hash.c_str()), _keyPairObj);
//
//     const BIGNUM* pr = BN_new();
//     const BIGNUM* ps = BN_new();
//
//     char* prChar;
//     char* psChar;
//
//     pr = ECDSA_SIG_get0_r(signature);
//     ps = ECDSA_SIG_get0_s(signature);
//
//     prChar = BN_bn2hex(pr);
//     psChar = BN_bn2hex(ps);
//
//     rSigVal = prChar;
//     sSigVal = psChar;
//
//     ECDSA_SIG_free(signature);
// }

// bool crypto::ECKeyPair::verify(std::string hash, std::string rSigVal, std::string sSigVal)
// {
//     ECDSA_SIG* signature = ECDSA_SIG_new();
//
//     BIGNUM* r = BN_new();
//     BIGNUM* s = BN_new();
//
//     BN_hex2bn(&r, rSigVal.c_str());
//     BN_hex2bn(&s, sSigVal.c_str());
//
//     ECDSA_SIG_set0(signature, r, s);
//
//     int success = ECDSA_do_verify((const unsigned char*)hash.c_str(), strlen(hash.c_str()), signature, _keyPairObj);
//
//     ECDSA_SIG_free(signature);
//
//     return success;
// }

namespace crypto
{
    namespace asym
    {
        // std::ostream& operator<<(std::ostream& out, ECKeyPair& data)
        // {
        //     out << "Private key: " << data.getPrivateKey() << std::endl;
        //     out << "CPublic key: " << data.getCPublicKey() << std::endl;
        //
        //     return out;
        // }
    }
}