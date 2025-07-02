#include "./Stealth.hpp"


crypto::StealthKeys::StealthKeys()
{   
    // _ecKeys = std::make_shared<ECKeyPair>();
    //
    // deriveViewKeys();
    genKeyPairObj();
}

bool crypto::StealthKeys::genKeyPairObj()
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

const BIGNUM* crypto::StealthKeys::getPrivKey() const
{
    if (EVP_PKEY_base_id(mKeyPairObj) != EVP_PKEY_EC)
        return nullptr;

    const EC_KEY* key = EVP_PKEY_get1_EC_KEY(mKeyPairObj);

    return EC_KEY_get0_private_key(key);
}

// crypto::StealthKeys::StealthKeys(std::string key)
// {
//     _ecKeys = std::make_shared<ECKeyPair>(KeyType::Private, key);
//
//     deriveViewKeys();
// }

// std::string crypto::StealthKeys::getPublicAddr() const
// {
//     return getPublicSpendKey() + getPublicViewKey();
// }
//
// std::string crypto::StealthKeys::genStealthAddr(std::string recipient, uint8_t index, std::string rStr)
// {
//     std::string pubSpend = recipient.substr(0, 66);
//     std::string pubView  = recipient.substr(66, 66);
//
//     std::string stealthAddr;
//
//     EC_GROUP* secp256k1Group = EC_GROUP_new_by_curve_name(NID_secp256k1);
//     const BIGNUM* order      = EC_GROUP_get0_order(secp256k1Group);
//     const BIGNUM* cofactor   = EC_GROUP_get0_cofactor(secp256k1Group);
//
//     BIGNUM*   r = BN_new();
//     EC_POINT* A = EC_POINT_new(secp256k1Group);
//     EC_POINT* B = EC_POINT_new(secp256k1Group);
//     EC_POINT* D = EC_POINT_new(secp256k1Group);
//     BIGNUM*   f = BN_new();
//     EC_POINT* F = EC_POINT_new(secp256k1Group);
//     EC_POINT* P = EC_POINT_new(secp256k1Group);
//
//     BN_hex2bn(&r, rStr.c_str());
//
//     A = EC_POINT_hex2point(secp256k1Group, pubView.c_str(), nullptr, nullptr);
//     B = EC_POINT_hex2point(secp256k1Group, pubSpend.c_str(), nullptr, nullptr);
//
//     D = utils::ec_mul(r, A);
//
//     std::string DStr = EC_POINT_point2hex(secp256k1Group, D, POINT_CONVERSION_COMPRESSED, nullptr);
//
//     f = utils::Hs(hash::SHA3(utils::hexToString(DStr) + std::to_string(index)));
//
//     F = utils::ec_mul(f, nullptr);
//
//     EC_POINT_add(secp256k1Group, P, F, B, nullptr);
//
//     stealthAddr = EC_POINT_point2hex(secp256k1Group, P, POINT_CONVERSION_COMPRESSED, nullptr);
//
//     EC_GROUP_free(secp256k1Group);
//
//     BN_free(r);
//     EC_POINT_free(A);
//     EC_POINT_free(B);
//     EC_POINT_free(D);
//     BN_free(f);
//     EC_POINT_free(F);
//     EC_POINT_free(P);
//
//     return stealthAddr;
// }
//
// std::string crypto::StealthKeys::deriveStealthKey(std::string addr, uint8_t index, std::string RStr) const
// {
//     EC_GROUP* secp256k1Group = EC_GROUP_new_by_curve_name(NID_secp256k1);
//     const BIGNUM* order      = EC_GROUP_get0_order(secp256k1Group);
//     const BIGNUM* cofactor   = EC_GROUP_get0_cofactor(secp256k1Group);
//
//     BIGNUM*   a = BN_new();
//     BIGNUM*   b = BN_new();
//     EC_POINT* R = EC_POINT_new(secp256k1Group);
//     EC_POINT* D = EC_POINT_new(secp256k1Group);
//     BIGNUM*   f = BN_new();
//     BIGNUM*   x = BN_new();
//
//     BN_hex2bn(&a, getPrivateViewKey().c_str());
//     BN_hex2bn(&b, getPrivateSpendKey().c_str());
//
//     R = EC_POINT_hex2point(secp256k1Group, RStr.c_str(), nullptr, nullptr);
//
//     D = utils::ec_mul(a, R);
//
//     std::string DStr = EC_POINT_point2hex(secp256k1Group, D, POINT_CONVERSION_COMPRESSED, nullptr);
//
//     f = utils::Hs(hash::SHA3(utils::hexToString(DStr) + std::to_string(index)));
//
//     BN_add(x, f, b);
//
//     std::string xStr = BN_bn2hex(x);
//
//     EC_GROUP_free(secp256k1Group);
//     BN_free(a);
//     BN_free(b);
//     EC_POINT_free(R);
//     EC_POINT_free(D);
//     BN_free(f);
//     BN_free(x);
//
//     return xStr;
// }
//
// bool crypto::StealthKeys::identifyOutput(std::string addr, uint8_t index, std::string RStr) const
// {
//     EC_GROUP* secp256k1Group = EC_GROUP_new_by_curve_name(NID_secp256k1);
//     const BIGNUM* order      = EC_GROUP_get0_order(secp256k1Group);
//     const BIGNUM* cofactor   = EC_GROUP_get0_cofactor(secp256k1Group);
//
//     BIGNUM*   a = BN_new();
//     EC_POINT* R = EC_POINT_new(secp256k1Group);
//     EC_POINT* B = EC_POINT_new(secp256k1Group);
//     EC_POINT* D = EC_POINT_new(secp256k1Group);
//     BIGNUM*   f = BN_new();
//     EC_POINT* F = EC_POINT_new(secp256k1Group);
//     EC_POINT* P = EC_POINT_new(secp256k1Group);
//
//     R = EC_POINT_hex2point(secp256k1Group, RStr.c_str(), nullptr, nullptr);
//     B = EC_POINT_hex2point(secp256k1Group, getPublicSpendKey().c_str(), nullptr, nullptr);
//
//     BN_hex2bn(&a, getPrivateViewKey().c_str());
//
//     D = utils::ec_mul(a, R);
//
//     std::string DStr = EC_POINT_point2hex(secp256k1Group, D, POINT_CONVERSION_COMPRESSED, nullptr);
//
//     f = utils::Hs(hash::SHA3(utils::hexToString(DStr) + std::to_string(index)));
//
//     F = utils::ec_mul(f, nullptr);
//
//     EC_POINT_add(secp256k1Group, P, F, B, nullptr);
//
//     std::string PStr = EC_POINT_point2hex(secp256k1Group, P, POINT_CONVERSION_COMPRESSED, nullptr);
//
//     EC_GROUP_free(secp256k1Group);
//     BN_free(a);
//     EC_POINT_free(R);
//     EC_POINT_free(B);
//     EC_POINT_free(D);
//     BN_free(f);
//     EC_POINT_free(F);
//     EC_POINT_free(P);
//
//     return PStr == addr;
// }
//
// bool crypto::StealthKeys::verifyOutputOwnership(std::string addr, std::string key) const
// {
//     EC_GROUP* curve = EC_GROUP_new_by_curve_name(NID_secp256k1);
//
//     EC_POINT* P = EC_POINT_new(curve);
//     BIGNUM*   x = BN_new();
//
//     BN_hex2bn(&x, key.c_str());
//
//     P = utils::ec_mul(x, nullptr);
//
//     std::string PStr = EC_POINT_point2hex(curve, P, POINT_CONVERSION_COMPRESSED, nullptr);
//
//     return addr == PStr;
// }
//
// std::string crypto::StealthKeys::deriveImage(std::string addr, uint8_t index, std::string RStr)
// {
//     std::string image;
//
//     EC_GROUP* group = EC_GROUP_new_by_curve_name(NID_secp256k1);
//
//     // Calculate Stealth Key(x) to Addr(P)
//     std::string stealthKey = deriveStealthKey(addr, index, RStr);
//
//     BIGNUM*   x    = BN_new();
//     EC_POINT* HpP  = EC_POINT_new(group);
//     EC_POINT* I    = EC_POINT_new(group);
//
//     BN_hex2bn(&x, stealthKey.c_str());
//
//     std::string HP = hash::SHA3(utils::hexToString(addr));
//
//     HpP = utils::ssvg_hash_to_curve(HP, group);
//     I = utils::ec_mul(x, HpP);
//
//     image = EC_POINT_point2hex(group, I, POINT_CONVERSION_COMPRESSED, nullptr);
//
//     EC_GROUP_free(group);
//     BN_free(x);
//     EC_POINT_free(HpP);
//     EC_POINT_free(I);
//
//     return image;
// }
//
// std::string crypto::StealthKeys::genTxPrivKey()
// {
//     EC_GROUP* group = EC_GROUP_new_by_curve_name(NID_secp256k1);
//     const BIGNUM* order = EC_GROUP_get0_order(group);
//
//     BIGNUM* r = BN_new();
//     BN_rand_range(r, order);
//
//     std::string rStr = BN_bn2hex(r);
//
//     EC_GROUP_free(group);
//     BN_free(r);
//
//     return rStr;
// }
//
// std::string crypto::StealthKeys::deriveTxPubKey(std::string rStr)
// {
//     EC_GROUP* group = EC_GROUP_new_by_curve_name(NID_secp256k1);
//
//     BIGNUM* r = BN_new();
//     EC_POINT* R = EC_POINT_new(group);
//
//     BN_hex2bn(&r, rStr.c_str());
//     R = utils::ec_mul(r, nullptr);
//
//     std::string RStr = EC_POINT_point2hex(group, R, POINT_CONVERSION_COMPRESSED, nullptr);
//
//     EC_GROUP_free(group);
//     BN_free(r);
//     EC_POINT_free(R);
//
//     return RStr;
// }
//
// void crypto::StealthKeys::deriveViewKeys()
// {
//     // Initalize secp256k1 group
//     EC_GROUP* secp256k1Group = EC_GROUP_new_by_curve_name(NID_secp256k1);
//
//     BIGNUM* privKeyScalar = BN_new();
//
//     // Hash private spend key
//     std::string privKeyHashed = hash::SHA3(utils::hexToString(_ecKeys->getPrivateKey()));
//
//     // Convert hashed value to a BIGNUM and reduce it to make it a valid secp256k1 scalar
//     BN_hex2bn(&privKeyScalar, privKeyHashed.c_str());
//     utils::sc_reduce32(privKeyScalar, secp256k1Group);
//
//     // Convert reduced scalar to hex and assign it to _privateViewKey
//     _privateViewKey = BN_bn2hex(privKeyScalar);
//
//     // Derive public view key from private view key
//     EC_POINT* pubKey = utils::ec_mul(privKeyScalar, nullptr);
//
//     _publicViewKey = EC_POINT_point2hex(secp256k1Group, pubKey, POINT_CONVERSION_COMPRESSED, nullptr);
// }
//
// namespace crypto
// {
//     std::ostream& operator<<(std::ostream& out, StealthKeys& data)
//     {
//         out << "Public Addr:   " << data.getPublicAddr()      << "\n";
//         out << "Private Spend: " << data.getPrivateSpendKey() << "\n";
//         out << "Public Spend:  " << data.getPublicSpendKey()  << "\n";
//         out << "Private View:  " << data.getPrivateViewKey()  << "\n";
//         out << "Public View    " << data.getPublicViewKey();
//
//         return out;
//     }
// }