#include "./Stealth.hpp"


crypto::StealthKeys::StealthKeys()
{   
    _ecKeys = std::make_shared<asym::ECKeyPair>();

    genViewKeys();
}

crypto::StealthKeys::StealthKeys(std::string key)
{
    _ecKeys = std::make_shared<asym::ECKeyPair>(KeyType::Private, key);

    genViewKeys();
}

std::string crypto::StealthKeys::getPublicAddr() const
{
    return getPublicSpendKey() + getPublicViewKey();
}

std::string crypto::StealthKeys::genStealthAddr(std::string recipient, std::string& RStr) const
{
    std::string pubSpend = recipient.substr(0, 66);
    std::string pubView  = recipient.substr(66, 66);

    std::string stealthAddr;

    EC_GROUP* secp256k1Group = EC_GROUP_new_by_curve_name(NID_secp256k1);
    const BIGNUM* order      = EC_GROUP_get0_order(secp256k1Group);
    const BIGNUM* cofactor   = EC_GROUP_get0_cofactor(secp256k1Group);

    BIGNUM*   r = BN_new();
    EC_POINT* R = EC_POINT_new(secp256k1Group);
    EC_POINT* A = EC_POINT_new(secp256k1Group);
    EC_POINT* B = EC_POINT_new(secp256k1Group);
    EC_POINT* D = EC_POINT_new(secp256k1Group);
    BIGNUM*   f = BN_new();
    EC_POINT* F = EC_POINT_new(secp256k1Group);
    EC_POINT* P = EC_POINT_new(secp256k1Group);

    BN_rand(r, 256, BN_RAND_TOP_ANY, BN_RAND_BOTTOM_ANY);
    utils::sc_reduce32(r, secp256k1Group);

    EC_POINT_hex2point(secp256k1Group, pubView.c_str(), A, nullptr);
    EC_POINT_hex2point(secp256k1Group, pubSpend.c_str(), B, nullptr);

    EC_POINT_mul(secp256k1Group, R, r, nullptr, nullptr, nullptr);

    RStr = EC_POINT_point2hex(secp256k1Group, R, POINT_CONVERSION_COMPRESSED, nullptr);

    EC_GROUP_set_generator(secp256k1Group, A, order, cofactor);

    EC_POINT_mul(secp256k1Group, D, r, nullptr, nullptr, nullptr);

    std::string DStr = EC_POINT_point2hex(secp256k1Group, D, POINT_CONVERSION_COMPRESSED, nullptr);
    std::string fStr = hash::SHA3(DStr);

    BN_hex2bn(&f, fStr.c_str());

    EC_GROUP_free(secp256k1Group);

    secp256k1Group = EC_GROUP_new_by_curve_name(NID_secp256k1);

    EC_POINT_mul(secp256k1Group, F, f, nullptr, nullptr, nullptr);

    EC_POINT_add(secp256k1Group, P, F, B, nullptr);

    stealthAddr = EC_POINT_point2hex(secp256k1Group, P, POINT_CONVERSION_COMPRESSED, nullptr);

    EC_GROUP_free(secp256k1Group);

    BN_free(r);
    EC_POINT_free(A);
    EC_POINT_free(B);
    EC_POINT_free(D);
    BN_free(f);
    EC_POINT_free(F);
    EC_POINT_free(P);

    return stealthAddr;
}

std::string crypto::StealthKeys::genStealthKey(std::string addr, std::string RStr) const
{
    EC_GROUP* secp256k1Group = EC_GROUP_new_by_curve_name(NID_secp256k1);
    const BIGNUM* order      = EC_GROUP_get0_order(secp256k1Group);
    const BIGNUM* cofactor   = EC_GROUP_get0_cofactor(secp256k1Group);

    BIGNUM*   a = BN_new();
    BIGNUM*   b = BN_new();
    EC_POINT* R = EC_POINT_new(secp256k1Group);
    EC_POINT* D = EC_POINT_new(secp256k1Group);
    BIGNUM*   f = BN_new();
    BIGNUM*   x = BN_new();

    BN_hex2bn(&a, getPrivateViewKey().c_str());
    BN_hex2bn(&b, getPrivateSpendKey().c_str());
    EC_POINT_hex2point(secp256k1Group, RStr.c_str(), R, nullptr);
    
    EC_GROUP_set_generator(secp256k1Group, R, order, cofactor);

    EC_POINT_mul(secp256k1Group, D, a, nullptr, nullptr, nullptr);

    std::string DStr = EC_POINT_point2hex(secp256k1Group, D, POINT_CONVERSION_COMPRESSED, nullptr);
    std::string fStr = hash::SHA3(DStr);

    BN_hex2bn(&f, fStr.c_str());

    BN_add(x, f, b);

    std::string xStr = BN_bn2hex(x);

    EC_GROUP_free(secp256k1Group);
    BN_free(a);
    BN_free(b);
    EC_POINT_free(R);
    EC_POINT_free(D);
    BN_free(f);
    BN_free(x);

    return xStr;
}

bool crypto::StealthKeys::verifyStealthAddr(std::string addr, std::string RStr) const
{
    EC_GROUP* secp256k1Group = EC_GROUP_new_by_curve_name(NID_secp256k1);
    const BIGNUM* order      = EC_GROUP_get0_order(secp256k1Group);
    const BIGNUM* cofactor   = EC_GROUP_get0_cofactor(secp256k1Group);

    BIGNUM*   a = BN_new();
    EC_POINT* R = EC_POINT_new(secp256k1Group);
    EC_POINT* B = EC_POINT_new(secp256k1Group);
    EC_POINT* D = EC_POINT_new(secp256k1Group);
    BIGNUM*   f = BN_new();
    EC_POINT* F = EC_POINT_new(secp256k1Group);
    EC_POINT* P = EC_POINT_new(secp256k1Group);

    EC_POINT_hex2point(secp256k1Group, RStr.c_str(), R, nullptr);
    EC_POINT_hex2point(secp256k1Group, getPublicSpendKey().c_str(), B, nullptr);

    BN_hex2bn(&a, getPrivateViewKey().c_str());

    EC_GROUP_set_generator(secp256k1Group, R, order, cofactor);

    EC_POINT_mul(secp256k1Group, D, a, nullptr, nullptr, nullptr);

    std::string DStr = EC_POINT_point2hex(secp256k1Group, D, POINT_CONVERSION_COMPRESSED, nullptr);
    std::string fStr = hash::SHA3(DStr);

    

    BN_hex2bn(&f, fStr.c_str());

    EC_GROUP_free(secp256k1Group);

    secp256k1Group = EC_GROUP_new_by_curve_name(NID_secp256k1);

    EC_POINT_mul(secp256k1Group, F, f, nullptr, nullptr, nullptr);

    EC_POINT_add(secp256k1Group, P, F, B, nullptr);

    std::string PStr = EC_POINT_point2hex(secp256k1Group, P, POINT_CONVERSION_COMPRESSED, nullptr);

    EC_GROUP_free(secp256k1Group);
    BN_free(a);
    EC_POINT_free(R);
    EC_POINT_free(B);
    EC_POINT_free(D);
    BN_free(f);
    EC_POINT_free(F);
    EC_POINT_free(P);

    return PStr == addr;
}

std::string crypto::StealthKeys::genImage(std::string addr, std::string RStr)
{
    std::string image;

    EC_GROUP* secp256k1Group = EC_GROUP_new_by_curve_name(NID_secp256k1);
    const BIGNUM* order      = EC_GROUP_get0_order(secp256k1Group);
    const BIGNUM* cofactor   = EC_GROUP_get0_cofactor(secp256k1Group);

    // Calculate Stealth Key(x) to Addr(P)
    std::string stealthKey = genStealthKey(addr, RStr);
    
    BIGNUM*   x    = BN_new();
    EC_POINT* HpP  = EC_POINT_new(secp256k1Group);
    EC_POINT* I    = EC_POINT_new(secp256k1Group);

    BN_hex2bn(&x, stealthKey.c_str());

    std::string HP = hash::SHA3(utils::hexToString(addr));

    HpP = utils::ssvg_hash_to_curve(HP, secp256k1Group);

    EC_GROUP_set_generator(secp256k1Group, HpP, order, cofactor);

    EC_POINT_mul(secp256k1Group, I, x, nullptr, nullptr, nullptr);

    EC_GROUP_free(secp256k1Group);
    secp256k1Group = EC_GROUP_new_by_curve_name(NID_secp256k1);

    image = EC_POINT_point2hex(secp256k1Group, I, POINT_CONVERSION_COMPRESSED, nullptr);

    EC_GROUP_free(secp256k1Group);
    BN_free(x);
    EC_POINT_free(HpP);
    EC_POINT_free(I);

    return image;
}

void crypto::StealthKeys::genViewKeys()
{
    // Initalize secp256k1 group and get its order
    EC_GROUP*     secp256k1Group = EC_GROUP_new_by_curve_name(NID_secp256k1);
    const BIGNUM* secp256k1Order = EC_GROUP_get0_order(secp256k1Group);

    BIGNUM* privKeyScalar = BN_new();

    // Hash private spend key
    std::string privKeyUnhashed = utils::hexToString(_ecKeys->getPrivateKey());
    std::string privKeyHashed = hash::SHA3(privKeyUnhashed);

    // Convert hashed value to a BIGNUM and reduce it to make it a valid secp256k1 scalar
    BN_hex2bn(&privKeyScalar, privKeyHashed.c_str());

    utils::sc_reduce32(privKeyScalar, secp256k1Group);

    // // Convert reduced scalar to hex and assign it to _privateViewKey
    char* privViewKey = BN_bn2hex(privKeyScalar);

    _privateViewKey = privViewKey;

    // Derive public view key from private view key
    EC_POINT* pubKey = EC_POINT_new(secp256k1Group);

    EC_POINT_mul(secp256k1Group, pubKey, privKeyScalar, nullptr, nullptr, nullptr);

    char* pubViewKey = EC_POINT_point2hex(secp256k1Group, pubKey, POINT_CONVERSION_COMPRESSED, nullptr);

    _publicViewKey = pubViewKey;
}

namespace crypto
{
    std::ostream& operator<<(std::ostream& out, StealthKeys& data)
    {
        out << "Private Spend: " << data.getPrivateSpendKey() << "\n";
        out << "Public Spend:  " << data.getPublicSpendKey()  << "\n";
        out << "Private View:  " << data.getPrivateViewKey()  << "\n";
        out << "Public View    " << data.getPublicViewKey();

        return out;
    }
}