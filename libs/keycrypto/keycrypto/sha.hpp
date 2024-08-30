#ifndef SHA_H
#define SHA_H

#include <iomanip>
#include <string>
#include <openssl/sha.h>
#include <openssl/evp.h>


namespace crypto
{
    namespace hash
    {
        bool SHA256(const std::string& unhashed, std::string& hashed);
        
        std::string SHA3(const std::string& unhashed);
    }   
}

#endif