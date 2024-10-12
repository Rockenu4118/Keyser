#ifndef CRYPTO_UTILS_H
#define CRYPTO_UTILS_H

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <ctime>

#include <openssl/ec.h>
#include <openssl/evp.h>
#include <openssl/bn.h>
#include <openssl/sha.h>
#include <openssl/rand.h>

typedef long int64_t;
typedef unsigned long uint64_t;

namespace crypto
{
    namespace utils
    {
        ///////////////////////////////////////////////////////////////////////////////////////////
        // Conversions
        ///////////////////////////////////////////////////////////////////////////////////////////

        unsigned char parse_hex(char c);

        void parse_string(unsigned char* arr, const std::string& s);

        std::string hexToString(const std::string& input);

        std::string uChar2Str(unsigned char* arr, int len);


        ///////////////////////////////////////////////////////////////////////////////////////////
        // Elliptic Curve Helpers
        ///////////////////////////////////////////////////////////////////////////////////////////

        void sc_reduce32(BIGNUM*& scalar, const EC_GROUP* curve);

        // Hash to EC scalar
        BIGNUM* Hs(const std::string hash);

        // SSVG hash to point
        EC_POINT* ssvg_hash_to_curve(const std::string hash, const EC_GROUP* curve);

        EC_POINT* ec_mul(BIGNUM* s, EC_POINT* P);

        ///////////////////////////////////////////////////////////////////////////////////////////
        // Random Gen
        ///////////////////////////////////////////////////////////////////////////////////////////

        BIGNUM* rand(size_t size);

        std::string randomBytes(size_t size);

        ///////////////////////////////////////////////////////////////////////////////////////////
        // Address Gen
        ///////////////////////////////////////////////////////////////////////////////////////////

        std::string keyserAddrGen(std::string pubSpend, std::string pubView);

        ///////////////////////////////////////////////////////////////////////////////////////////
        // Print Structures
        ///////////////////////////////////////////////////////////////////////////////////////////

        void print(BIGNUM* bn);

        void print(EC_POINT* point);

        
    }
}

#endif