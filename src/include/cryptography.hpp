#ifndef CRYPTOGRAPHY_H
#define CRYPTOGRAPHY_H

#include <string>


namespace cryptography
{
    // Hashing
    bool sha256(const std::string& unhashed, std::string& hashed);

    // Asymmetric encryption
    bool ecKeys(std::string& privateKey, std::string& uPublicKey, std::string& cPublicKey);

    // Other
    std::string hexToString(const std::string& input);
}

#endif