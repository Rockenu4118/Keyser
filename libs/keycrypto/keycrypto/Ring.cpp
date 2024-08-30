#include "./Ring.hpp"


crypto::Ring::Ring(std::vector<std::string> keys, std::string signingKey) : _keys(keys), _N(keys.size())
{
    // Init Curve
    _curve = EC_GROUP_new_by_curve_name(NID_secp256k1);

    // Find index of shuffled signingKey
    auto iter = find(_keys.begin(), _keys.end(), signingKey);
    _signingKeyI  = iter - _keys.begin();

    // Init All Bignums and EC_POINT's
    _e.resize(_N);
    _s.resize(_N);
    _P.resize(_N);

    for (int i = 0 ; i < _N ; i++)
    {
        // Init e
        _e.at(i) = BN_new();

        // Init s
        _s.at(i) = BN_new();

        // Init P
        _P.at(i) = EC_POINT_new(_curve);
        _P.at(i) = EC_POINT_hex2point(_curve, _keys.at(i).c_str(), nullptr, nullptr);
    }
}

crypto::Ring::~Ring()
{
    freeRing();
}

std::vector<std::string> crypto::Ring::sign(std::string hash, std::string stealthKey)
{
    std::vector<std::string> sig;

    // Init group
    EC_GROUP* secp256k1 = EC_GROUP_new_by_curve_name(NID_secp256k1);
    const BIGNUM* order = EC_GROUP_get0_order(secp256k1);
    BN_CTX* ctx = BN_CTX_new();

    // Init variables for ring sig equation
    BIGNUM*   alpha = BN_new();
    EC_POINT* Q     = EC_POINT_new(secp256k1);
    BIGNUM*   x     = BN_new();

    for (int i = 0 ; i < _N ; i++)
    {
        BN_rand(_s.at(i), 256, BN_RAND_TOP_ANY, BN_RAND_BOTTOM_ANY);
        utils::sc_reduce32(_s.at(i), _curve);
    }

    // Gen random alpha and calc Q
    BN_rand(alpha, 256, BN_RAND_TOP_ANY, BN_RAND_BOTTOM_ANY);
    utils::sc_reduce32(alpha, secp256k1);

    Q = utils::ec_mul(alpha, nullptr);

    // Convert x
    BN_hex2bn(&x, stealthKey.c_str());

    std::string QStr = EC_POINT_point2hex(secp256k1, Q, POINT_CONVERSION_COMPRESSED, nullptr);

    // if j = 1, then, e_2 = Hs(M || qG)
    _e.at((_signingKeyI + 1) % _N) = utils::Hs(hash::SHA3(utils::hexToString(hash + QStr)));

    int i = _signingKeyI + 1;
    int j = 0;

    while (j < _N - 1)
    {
        int prev = i % _N;
        int curr = (i + 1) % _N;

        // e = Hs(M || R)
        // R = sG + sP
        _e.at(curr) = calc_e(hash, _s.at(prev), _e.at(prev), _P.at(prev));

        i++; j++;
    }
    
    // BIGNUM* ej_xj = BN_new();
    BN_mod_mul(_s.at(_signingKeyI), _e.at(_signingKeyI), x, order, ctx);
    BN_mod_sub(_s.at(_signingKeyI), alpha, _s.at(_signingKeyI), order, ctx);
    BN_mod_add(_s.at(_signingKeyI), _s.at(_signingKeyI), order, order, ctx);

    // e0
    sig.push_back(BN_bn2hex(_e.at(0)));

    // s0, s1,...sN
    for (const auto& s : _s)
        sig.push_back(BN_bn2hex(s));

    EC_GROUP_free(secp256k1);
    BN_CTX_free(ctx);
    BN_free(alpha);
    EC_POINT_free(Q);
    BN_free(x);

    return sig;
}

bool crypto::Ring::verify(std::string hash, std::vector<std::string> sig, std::vector<std::string> publicAddrs)
{
    if (sig.size() - 1 != publicAddrs.size())
        return false;

    EC_GROUP* curve = EC_GROUP_new_by_curve_name(NID_secp256k1);

    const int N = publicAddrs.size();

    std::string e0;
    std::string edash;

    std::vector<BIGNUM*>   e(N);
    std::vector<BIGNUM*>   s(N);
    std::vector<EC_POINT*> P(N);

    for (int i = 0 ; i < N ; i++)
    {
        e.at(i) = BN_new();

        s.at(i) = BN_new();
        BN_hex2bn(&s.at(i), sig.at(i + 1).c_str());

        P.at(i) = EC_POINT_new(curve);
        P.at(i) = EC_POINT_hex2point(curve, publicAddrs.at(i).c_str(), nullptr, nullptr);
    }

    BN_hex2bn(&e.at(0), sig.at(0).c_str());
    e0 = BN_bn2hex(e.at(0));

    for (int i = 0 ; i < N ; i++)
    {
        int prev = i % N;
        int curr = (i + 1) % N;

        e.at(curr) = calc_e(hash, s.at(prev), e.at(prev), P.at(prev));
    }

    edash = BN_bn2hex(e.at(0));

    // Free memory
    EC_GROUP_free(curve);
    
    for (int i = 0 ; i < N ; i++)
    {
        BN_free(e.at(i));
        BN_free(s.at(i));
        EC_POINT_free(P.at(i));
    }

    // Verify e0 = e'
    return e0 == edash;
}

void crypto::Ring::printRing() const
{   
    std::cout << "-----Ring-----" << "\n";
    std::cout << "N: " << _N << ", Index: " << _signingKeyI << "\n";

    for (int i = 0 ; i < _N ; i++)
        std::cout << "#" << i << ": " << _keys.at(i) << "\n";
    std::cout << "-----Ring-----" << "\n";
}

void crypto::Ring::freeRing()
{
    for (int i = 0 ; i < _N ; i++)
    {
        BN_free(_e.at(i));
        BN_free(_s.at(i));
        EC_POINT_free(_P.at(i));
    }

    EC_GROUP_free(_curve);
}

BIGNUM* crypto::Ring::calc_e(std::string hash, BIGNUM* s, BIGNUM* e, EC_POINT* P)
{
    EC_GROUP* curve = EC_GROUP_new_by_curve_name(NID_secp256k1);

    EC_POINT* S = EC_POINT_new(curve);
    EC_POINT* E = EC_POINT_new(curve);
    EC_POINT* R = EC_POINT_new(curve); 

    S = utils::ec_mul(s, nullptr);
    E = utils::ec_mul(e, P);

    EC_POINT_add(curve, R, S, E, nullptr);

    std::string RStr = EC_POINT_point2hex(curve, R, POINT_CONVERSION_COMPRESSED, nullptr);

    EC_GROUP_free(curve);
    EC_POINT_free(S);
    EC_POINT_free(E);
    EC_POINT_free(R);

    return utils::Hs(hash::SHA3(utils::hexToString(hash + RStr)));
}
