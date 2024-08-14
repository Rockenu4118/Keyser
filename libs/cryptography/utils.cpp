#include "./utils.hpp"

unsigned char crypto::utils::parse_hex(char c)
{
    if ('0' <= c && c <= '9') return c - '0';
    if ('A' <= c && c <= 'F') return c - 'A' + 10;
    if ('a' <= c && c <= 'f') return c - 'a' + 10;
    std::abort();
}

void crypto::utils::parse_string(unsigned char* arr, const std::string& s)
{
    if (s.size() % 2 != 0) std::abort();
    std::vector<unsigned char> result(s.size() / 2);

    for (std::size_t i = 0; i != s.size() / 2; ++i)
        arr[i] = 16 * parse_hex(s[2 * i]) + parse_hex(s[2 * i + 1]);
}

void crypto::utils::sc_reduce32(BIGNUM*& scalar, const EC_GROUP* curve)
{
    const BIGNUM* order = EC_GROUP_get0_order(curve);
    BN_CTX* bnCtx = BN_CTX_new();
    
    BN_mod(scalar, scalar, order, bnCtx);

    BN_CTX_free(bnCtx);
}

std::string crypto::utils::hexToString(const std::string& input)
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

EC_POINT* crypto::utils::ssvg_hash_to_curve(std::string hash, const EC_GROUP* curve) 
{
    BIGNUM* x = BN_new();
    BN_hex2bn(&x, hash.c_str());

    // Create initial points
    EC_POINT* finalP = EC_POINT_new(curve);
    EC_POINT* P = EC_POINT_new(curve);
    EC_POINT_set_compressed_coordinates(curve, P, x, 0, nullptr);
    EC_POINT* Q = EC_POINT_dup(P, curve); // Duplicate P for efficiency

    while (true)
    {
        EC_POINT_set_compressed_coordinates(curve, P, x, 0, nullptr);

        if (EC_POINT_is_at_infinity(curve, P))
            BN_add_word(x, 1);
        else
            break;
    }
    
    unsigned char* hashHex = new unsigned char[32];
    parse_string(hashHex, hash);

    // Iterative process
    for (int i = 0 ; i < 256 ; i++ ) 
    {
        bool bit = (hashHex[i / 8] >> (7 - (i % 8))) & 1;

        if (bit) {
            EC_POINT_add(curve, P, P, Q, NULL); // P += Q
        } else {
            EC_POINT_dbl(curve, P, P, NULL); // P = 2P
        }

        if (!EC_POINT_is_at_infinity(curve, P))
            finalP = EC_POINT_dup(P, curve);
        else
            break;
    }

    EC_POINT_free(P);
    EC_POINT_free(Q);
    BN_free(x);
    delete hashHex;

    return finalP;
}