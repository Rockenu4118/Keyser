#ifndef RING_H
#define RING_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <openssl/ec.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include "./cryptoUtils.hpp"
#include "./sha.hpp"


namespace crypto
{
    class Ring
    {
        public:
            Ring() = delete;

            static std::vector<std::string> sign(std::string hash, std::vector<std::string> outputs, std::string realOutput, std::string stealthKey, std::string keyImage);

            static bool verify(std::string hash, std::vector<std::string> sig, std::vector<std::string> publicAddrs, std::string keyImage);

            static void printSig(std::vector<std::string> sig);

        private:
            static BIGNUM* calc_e(std::string hash, BIGNUM* s, BIGNUM* e, EC_POINT* P, EC_POINT* I);
    };
}

#endif