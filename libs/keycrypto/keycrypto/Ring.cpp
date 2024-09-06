#include "./Ring.hpp"


std::vector<std::string> crypto::Ring::sign(std::string hash, std::vector<std::string> outputs, std::string realOutput, std::string stealthKey, std::string keyImage)
{
    std::vector<std::string> sig;

    EC_GROUP* group = EC_GROUP_new_by_curve_name(NID_secp256k1);
    const BIGNUM* order = EC_GROUP_get0_order(group);
    BN_CTX* bnCtx = BN_CTX_new();

    const int N = outputs.size();

    // Find index of shuffled signingKey
    auto iter = find(outputs.begin(), outputs.end(), realOutput);
    int j = iter - outputs.begin();

    std::vector<BIGNUM*>   s(N);
    std::vector<BIGNUM*>   e(N);
    std::vector<EC_POINT*> P(N);

    for (int i = 0 ; i < N ; i++)
    {
        // Init e
        e.at(i) = BN_new();

        // Init s
        s.at(i) = BN_new();
        if (i != j)
            BN_rand_range(s.at(i), order);

        // Init P
        P.at(i) = EC_POINT_new(group);
        P.at(i) = EC_POINT_hex2point(group, outputs.at(i).c_str(), nullptr, nullptr);
    }

    // Init variables for ring sig equation
    BIGNUM*   alpha = BN_new();
    EC_POINT* Q     = EC_POINT_new(group);
    EC_POINT* Q2    = EC_POINT_new(group);
    EC_POINT* HpPj  = EC_POINT_new(group);
    BIGNUM*   x     = BN_new();
    EC_POINT* I     = EC_POINT_new(group);

    // Gen random alpha and calc Q
    BN_rand_range(alpha, order);

    // Convert x
    BN_hex2bn(&x, stealthKey.c_str());
    I = EC_POINT_hex2point(group, keyImage.c_str(), nullptr, nullptr);

    // Q = alphaG
    Q = utils::ec_mul(alpha, nullptr);
    std::string QStr = EC_POINT_point2hex(group, Q, POINT_CONVERSION_COMPRESSED, nullptr);

    // Q2 = alphaHp(Pj)
    HpPj = utils::ssvg_hash_to_curve(hash::SHA3(utils::hexToString(realOutput)), group);
    Q2 = utils::ec_mul(alpha, HpPj);
    std::string Q2Str = EC_POINT_point2hex(group, Q2, POINT_CONVERSION_COMPRESSED, nullptr);

    // if j = 1, then, e_2 = Hs(M || alphaG || alphaHp(Pj)) = Hs(M || Q || Q2)
    e.at((j + 1) % N) = utils::Hs(hash::SHA3(utils::hexToString(hash + QStr + Q2Str)));

    int i = j + 1;
    int count = 0;

    while (count < N - 1)
    {
        int prev = i % N;
        int curr = (i + 1) % N;

        // e = Hs(M || R || R2)
        // R = sG + eP
        // R2 = sHp(Pi) + eI
        e.at(curr) = calc_e(hash, s.at(prev), e.at(prev), P.at(prev), I);

        i++; count++;
    }

    BN_mod_mul(s.at(j), e.at(j), x, order, bnCtx);
    BN_mod_sub(s.at(j), alpha, s.at(j), order, bnCtx);

    // e0
    sig.push_back(BN_bn2hex(e.at(0)));

    // s0, s1,...sN
    for (const auto& sVal : s)
        sig.push_back(BN_bn2hex(sVal));

    EC_GROUP_free(group);
    BN_CTX_free(bnCtx);
    BN_free(alpha);
    EC_POINT_free(Q);
    EC_POINT_free(Q2);
    EC_POINT_free(HpPj);
    BN_free(x);
    EC_POINT_free(I);

    for (int i = 0 ; i < N ; i++)
    {
        BN_free(e.at(i));
        BN_free(s.at(i));
        EC_POINT_free(P.at(i));
    }

    return sig;
}

bool crypto::Ring::verify(std::string hash, std::vector<std::string> sig, std::vector<std::string> publicAddrs, std::string keyImage)
{
    if (sig.size() - 1 != publicAddrs.size())
        return false;

    EC_GROUP* group = EC_GROUP_new_by_curve_name(NID_secp256k1);
    BN_CTX* bnCtx = BN_CTX_new();

    const int N = publicAddrs.size();

    std::string e0;
    std::string edash;

    std::vector<BIGNUM*>   e(N);
    std::vector<BIGNUM*>   s(N);
    std::vector<EC_POINT*> P(N);
    EC_POINT* I = EC_POINT_new(group);
    I = EC_POINT_hex2point(group, keyImage.c_str(), nullptr, nullptr);

    for (int i = 0 ; i < N ; i++)
    {
        e.at(i) = BN_new();

        s.at(i) = BN_new();
        BN_hex2bn(&s.at(i), sig.at(i + 1).c_str());

        P.at(i) = EC_POINT_new(group);
        P.at(i) = EC_POINT_hex2point(group, publicAddrs.at(i).c_str(), nullptr, nullptr);
    }

    BN_hex2bn(&e.at(0), sig.at(0).c_str());
    e0 = BN_bn2hex(e.at(0));

    for (int i = 0 ; i < N ; i++)
    {
        int prev = i % N;
        int curr = (i + 1) % N;

        e.at(curr) = calc_e(hash, s.at(prev), e.at(prev), P.at(prev), I);
    }

    edash = BN_bn2hex(e.at(0));

    // Free memory
    EC_GROUP_free(group);
    BN_CTX_free(bnCtx);
    EC_POINT_free(I);
    
    for (int i = 0 ; i < N ; i++)
    {
        BN_free(e.at(i));
        BN_free(s.at(i));
        EC_POINT_free(P.at(i));
    }

    // Verify e0 = e'
    return e0 == edash;
}

void crypto::Ring::printSig(std::vector<std::string> sig)
{
    std::cout << "-----Ring Sig-----\n";
    std::cout << "e0: " << sig.at(0) << "\n";
    for (int i = 1 ; i < sig.size() ; i++)
    {
        std::cout << "s" << i << ": " << sig.at(i) << "\n";
    }
    std::cout << "-----Ring Sig-----\n";
}

BIGNUM* crypto::Ring::calc_e(std::string hash, BIGNUM* s, BIGNUM* e, EC_POINT* P, EC_POINT* I)
{
    EC_GROUP* group = EC_GROUP_new_by_curve_name(NID_secp256k1);

    EC_POINT* S   = EC_POINT_new(group);
    EC_POINT* S2  = EC_POINT_new(group);
    EC_POINT* E   = EC_POINT_new(group);
    EC_POINT* E2  = EC_POINT_new(group);
    EC_POINT* R   = EC_POINT_new(group);
    EC_POINT* R2  = EC_POINT_new(group);
    EC_POINT* HpP = EC_POINT_new(group);

    S = utils::ec_mul(s, nullptr);
    E = utils::ec_mul(e, P);

    EC_POINT_add(group, R, S, E, nullptr);

    std::string PStr = EC_POINT_point2hex(group, P, POINT_CONVERSION_COMPRESSED, nullptr);
    HpP = utils::ssvg_hash_to_curve(hash::SHA3(utils::hexToString(PStr)), group);
    S2 = utils::ec_mul(s, HpP);

    E2 = utils::ec_mul(e, I);

    EC_POINT_add(group, R2, S2, E2, nullptr);

    std::string RStr = EC_POINT_point2hex(group, R, POINT_CONVERSION_COMPRESSED, nullptr);
    std::string R2Str = EC_POINT_point2hex(group, R2, POINT_CONVERSION_COMPRESSED, nullptr);

    EC_GROUP_free(group);
    EC_POINT_free(S);
    EC_POINT_free(S2);
    EC_POINT_free(E);
    EC_POINT_free(E2);
    EC_POINT_free(R);
    EC_POINT_free(R2);
    EC_POINT_free(HpP);

    return utils::Hs(hash::SHA3(utils::hexToString(hash + RStr + R2Str)));
}
