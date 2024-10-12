#include "./cryptoUtils.hpp"

unsigned char crypto::utils::parse_hex(char c)
{
    if ('0' <= c && c <= '9') return c - '0';
    if ('A' <= c && c <= 'F') return c - 'A' + 10;
    if ('a' <= c && c <= 'f') return c - 'a' + 10;
    std::abort();
}

void crypto::utils::parse_string(unsigned char* arr, const std::string& s)
{
    if (s.size() % 2 != 0)
    {
        std::cout << "Uneven length\n";
    }
    std::vector<unsigned char> result(s.size() / 2);

    for (std::size_t i = 0; i != s.size() / 2; ++i)
        arr[i] = 16 * parse_hex(s[2 * i]) + parse_hex(s[2 * i + 1]);
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

std::string crypto::utils::uChar2Str(unsigned char* arr, int len)
{
    std::stringstream ss;
    for(unsigned int i = 0; i < len; ++i)
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)arr[i];
    }
    
    return ss.str();
}

void crypto::utils::sc_reduce32(BIGNUM*& scalar, const EC_GROUP* curve)
{
    const BIGNUM* order = EC_GROUP_get0_order(curve);
    BN_CTX* bnCtx = BN_CTX_new();
    
    BN_mod(scalar, scalar, order, bnCtx);

    BN_CTX_free(bnCtx);
}

BIGNUM* crypto::utils::Hs(std::string hash)
{   
    BIGNUM* s = BN_new();
    BN_hex2bn(&s, hash.c_str());
    return s;
}

EC_POINT* crypto::utils::ssvg_hash_to_curve(std::string hash, const EC_GROUP* curve) 
{
    BIGNUM* x = BN_new();
    BN_hex2bn(&x, hash.c_str());

    // Create initial points
    EC_POINT* finalP = EC_POINT_new(curve);
    EC_POINT* P = EC_POINT_new(curve);
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
    delete[] hashHex;

    return finalP;
}

EC_POINT* crypto::utils::ec_mul(BIGNUM* s, EC_POINT* P = nullptr)
{
    EC_GROUP* curve = EC_GROUP_new_by_curve_name(NID_secp256k1);
    EC_POINT* S = EC_POINT_new(curve);

    if (P == nullptr)
    {
        EC_POINT_mul(curve, S, s, nullptr, nullptr, nullptr);
    }
    else
    {
        const BIGNUM* order = EC_GROUP_get0_order(curve);
        const BIGNUM* cofactor = EC_GROUP_get0_cofactor(curve);
        EC_GROUP_set_generator(curve, P, order, cofactor);
        EC_POINT_mul(curve, S, s, nullptr, nullptr, nullptr);
    }

    EC_GROUP_free(curve);

    return S;
}

BIGNUM* crypto::utils::rand(size_t size)
{
    BIGNUM* r = BN_new();
    BN_rand(r, size, BN_RAND_TOP_ANY, BN_RAND_BOTTOM_ANY);
    return r;
}

std::string crypto::utils::randomBytes(size_t size)
{
    unsigned char arr[size];
    RAND_bytes(arr, size);
    return uChar2Str(arr, size);
}

std::string crypto::utils::keyserAddrGen(std::string pubSpend, std::string pubView)
{
    return pubSpend + pubView;
}

void crypto::utils::print(BIGNUM* bn)
{
    std::cout << BN_bn2hex(bn) << "\n";
}

void crypto::utils::print(EC_POINT* point)
{
    EC_GROUP* group = EC_GROUP_new_by_curve_name(NID_secp256k1);
    std::cout << EC_POINT_point2hex(group, point, POINT_CONVERSION_COMPRESSED, nullptr) << "\n";
    EC_GROUP_free(group);
}