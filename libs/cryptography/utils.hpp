#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <ctime>

#include <openssl/ec.h>
#include <openssl/evp.h>
#include <openssl/bn.h>
#include <openssl/sha.h>

typedef long int64_t;
typedef unsigned long uint64_t;

namespace crypto
{
    namespace utils
    {
        unsigned char parse_hex(char c);
        void          parse_string(unsigned char* arr, const std::string& s);

        void sc_reduce32(BIGNUM*& scalar, const EC_GROUP* curve);

        EC_POINT* ssvg_hash_to_curve(const std::string hash, const EC_GROUP* curve);

        std::string hexToString(const std::string& input);
    }
}