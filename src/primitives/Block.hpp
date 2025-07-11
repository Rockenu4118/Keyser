#ifndef BLOCK_H
#define BLOCK_H

#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include <openssl/sha.h>

#include "./Transaction.hpp"


namespace keyser
{
    struct BlockHeader
    {
        uint64_t index{};
        uint64_t version{};
        std::array<uint8_t, SHA256_DIGEST_LENGTH> prevHash{};
        std::array<uint8_t, SHA256_DIGEST_LENGTH> merkleRoot{};
        uint64_t time{};
        uint64_t nonce{};
    };

    void hashHeader(BlockHeader& header, unsigned char hash[]);



    struct Block : BlockHeader
    {
        std::vector<Transaction> transactions{};
    };

    Block newBlock(uint64_t index, uint64_t time, unsigned char prevHash[], Transaction reward);

    BlockHeader getHeader(const Block& block);

    void calcMerkleRoot(std::vector<Transaction>& transactions, unsigned char merkleRoot[]);
}

#endif