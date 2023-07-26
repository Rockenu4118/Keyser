#ifndef HASH_H
#define HASH_H

#include <string>


namespace hash
{
    bool sha256(const std::string& unhashed, std::string& hashed);
}

#endif