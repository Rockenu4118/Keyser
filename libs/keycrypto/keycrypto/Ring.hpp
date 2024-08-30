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
#include "./utils.hpp"
#include "./sha.hpp"


namespace crypto
{
    class Ring
    {
        public:
            Ring(std::vector<std::string> fakeKeys, std::string signingKey);

            ~Ring();

            std::vector<std::string> sign(std::string hash, std::string stealthKey);

            static bool verify(std::string hash, std::vector<std::string> sig, std::vector<std::string> publicAddrs);

            void printRing() const;

        private:
            void freeRing();

            static BIGNUM* calc_e(std::string hash, BIGNUM* s, BIGNUM* e, EC_POINT* P);

            std::vector<std::string> _keys;
            
            int _signingKeyI;

            EC_GROUP* _curve;

            const int _N;

            std::vector<BIGNUM*>   _e;
            std::vector<BIGNUM*>   _s;
            std::vector<EC_POINT*> _P;
    };
}

#endif