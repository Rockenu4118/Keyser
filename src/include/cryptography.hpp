#ifndef CRYPTOGRAPHY_H
#define CRYPTOGRAPHY_H

#include <string>
#include <openssl/ec.h>


namespace cryptography
{
    // Hashing
    bool sha256(const std::string& unhashed, std::string& hashed);

    // Other
    std::string hexToString(const std::string& input);
}

#endif