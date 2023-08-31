#ifndef CRYPTOGRAPHY_H
#define CRYPTOGRAPHY_H

#include <string>


namespace cryptography
{
    // Hashing
    bool sha256(const std::string& unhashed, std::string& hashed);    
}

#endif