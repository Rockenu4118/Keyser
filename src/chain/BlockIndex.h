#ifndef BLOCKINDEX_H
#define BLOCKINDEX_H

#include <boost/multiprecision/cpp_int.hpp>

using boost::multiprecision::uint256_t;

namespace keyser
{
    struct BlockIndex {
        uint256_t hash;





        // Block Header
        uint32_t mVersion;
        uint256_t mHashMerkleRoot;
        uint32_t mTime;
        uint256_t mNonce;



    };
}




#endif //BLOCKINDEX_H
