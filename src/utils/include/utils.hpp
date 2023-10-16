#ifndef UTILS_H
#define UTILS_H

#include <string>

namespace keyser
{
    namespace utils
    {
        std::string pubKeytoAddress(const std::string& uPublicKey);
        std::string hexToString(const std::string& input);
    }
    
}

#endif