#ifndef CHAIN_H
#define CHAIN_H

#include <vector>
#include <memory>
#include <thread>
#include <unordered_map>
#include <boost/multiprecision/cpp_int.hpp>
#include <openssl/sha.h>

#include "../primitives/Block.hpp"
#include "./BlockIndex.h"
#include "./Mempool.hpp"

using boost::multiprecision::uint256_t;


namespace keyser 
{
    class Node;

    class ChainIndex
    {   
    public:
        // Main chain
        std::vector<BlockIndex*> mChain;

        // Map of all blocks in existence
        std::unordered_map<uint256_t, BlockIndex*> mMapBlockIndex;

        ChainIndex();

        // Initialize the one hardcoded block and add it to the chain
        void createGenesisBlock();

        void processBlock(Block& block);

        BlockIndex* operator[](const uint32_t height) const { return mChain.at(height); }

        BlockIndex* getTip();

        BlockIndex* getBlock(std::string hash);
            
            uint calcDifficulty() const;
            uint calcReward() const;
            uint32_t getHeight() const;

            // std::map<uint64_t, std::array<uint8_t, SHA256_DIGEST_LENGTH>> _blockIndex;
            //
            // std::unordered_map<std::array<uint8_t, SHA256_DIGEST_LENGTH>, BlockHeader, Hash256Hasher> _headers;
            //
            // std::unordered_map<std::array<uint8_t, SHA256_DIGEST_LENGTH>, Block, Hash256Hasher> _blocks;
    };
}

#endif