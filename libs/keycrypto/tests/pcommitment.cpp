#include <openssl/evp.h>
#include "../keycrypto/PCommitment.hpp"
#include "../keycrypto/sha.hpp"
#include "../keycrypto/utils.hpp"

void commitment_test()
{
    EC_GROUP* group = EC_GROUP_new_by_curve_name(NID_secp256k1);
    const EC_POINT* G = EC_GROUP_get0_generator(group);

    std::string gHex = EC_POINT_point2hex(group, G, POINT_CONVERSION_COMPRESSED, nullptr);

    std::cout << "G: " << gHex << std::endl;

    std::string Ghash = crypto::hash::SHA3(crypto::utils::hexToString(gHex));


    EC_POINT* H = crypto::utils::ssvg_hash_to_curve(Ghash, group);
    std::cout << "Is on curve: " << EC_POINT_is_on_curve(group, H, nullptr) << std::endl;
    std::string hHash = EC_POINT_point2hex(group, H, POINT_CONVERSION_COMPRESSED, nullptr);
    std::cout << "H: " << hHash << std::endl;

    std::vector<uint64_t> vals;
    vals.push_back(10);
    vals.push_back(5);
    vals.push_back(25);

    std::vector<std::string> RVals;

    crypto::PCommitment pc;

    std::string commitment = pc.genCommitment(vals, RVals);

    std::cout << "Commitment: " << commitment << "\n";

    bool valid = pc.verifyCommitment("", "");

    std::cout << "Valid C Out: " << valid << "\n";

    std::cout << "-----R Vals-----\n";
    for (const auto& R : RVals)
        std::cout << R << "\n";

}