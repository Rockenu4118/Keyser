#ifndef PCOMMITMENT_H
#define PCOMMITMENT_H

#include <string>
#include <vector>
#include <openssl/evp.h>
#include <openssl/ec.h>
#include "./aes.hpp"
#include "./utils.hpp"
#include "./sha.hpp"

namespace crypto
{
    class PCommitment
    {
        public:
            PCommitment() = default;

            std::string genCommitment(std::vector<uint64_t> vals, std::vector<std::string>& RVals);

            bool verifyCommitment(std::string cIn, std::string cOut);

            void decryptCommitment(std::string commitment);

        private:
            EC_POINT* H();

    };
}

#endif

