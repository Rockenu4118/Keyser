#include "./PCommitment.hpp"


std::string crypto::PCommitment::genCommitment(std::vector<uint64_t> vals, std::vector<std::string>& RVals)
{
    std::string CStr;
    const int N = vals.size();

    EC_GROUP* group = EC_GROUP_new_by_curve_name(NID_secp256k1);
    const BIGNUM* order = EC_GROUP_get0_order(group);
    BN_CTX* bnCtx = BN_CTX_new();

    const EC_POINT* G = EC_POINT_new(group);
    EC_POINT* H = EC_POINT_new(group);

    G = EC_GROUP_get0_generator(group);

    EC_POINT* finalC = EC_POINT_new(group);

    std::vector<EC_POINT*> C(N);
    std::vector<BIGNUM*>   v(N);
    std::vector<BIGNUM*>   r(N);

    H = utils::ssvg_hash_to_curve(hash::SHA3(utils::hexToString(EC_POINT_point2hex(group, G, POINT_CONVERSION_COMPRESSED, nullptr))), group);

    for (int i = 0 ; i < N ; i++)
    {
        v.at(i) = BN_new();
        BN_add_word(v.at(i), vals.at(i));

        r.at(i) = BN_new();
        BN_rand_range(r.at(i), order);

        C.at(i) = EC_POINT_new(group);
        EC_POINT* V = utils::ec_mul(v.at(i), H);
        EC_POINT* R = utils::ec_mul(r.at(i), nullptr);
        EC_POINT_add(group, C.at(i), V, R, bnCtx);
        EC_POINT_add(group, finalC, finalC, C.at(i), nullptr);

        // Push rG = R, vals to passed vector
        RVals.push_back(EC_POINT_point2hex(group, R, POINT_CONVERSION_COMPRESSED, nullptr));

        // Free memory
        EC_POINT_free(V);
        EC_POINT_free(R);
    }

    CStr = EC_POINT_point2hex(group, finalC, POINT_CONVERSION_COMPRESSED, nullptr);

    // Free memory
    EC_GROUP_free(group);
    BN_CTX_free(bnCtx);
    EC_POINT_free(H);
    EC_POINT_free(finalC);

    for (int i = 0 ; i < N ; i++)
    {
        EC_POINT_free(C.at(i));
        BN_free(v.at(i));
        BN_free(r.at(i));
    }

    return CStr;
}

bool crypto::PCommitment::verifyCommitment(std::string cIn, std::string cOut)
{
    EC_GROUP* group = EC_GROUP_new_by_curve_name(NID_secp256k1);
    EC_POINT* bru = EC_POINT_new(group);
    utils::print(bru);


    return true;
}

EC_POINT* crypto::PCommitment::H()
{
    EC_GROUP* group = EC_GROUP_new_by_curve_name(NID_secp256k1);
    const EC_POINT* G = EC_GROUP_get0_generator(group);
    std::string GStr = EC_POINT_point2hex(group, G, POINT_CONVERSION_COMPRESSED, nullptr);

    EC_POINT* H = utils::ssvg_hash_to_curve(hash::SHA3(utils::hexToString(GStr)), group);

    EC_GROUP_free(group);

    return H;
}