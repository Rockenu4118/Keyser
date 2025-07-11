#include <iostream>
#include <ctime>
#include <vector>
#include <string>

#include <nlohmann/json.hpp>

#include "Block.hpp"
#include "./Transaction.hpp"
#include "../utils/utils.hpp"
#include "../data/version.hpp"


void keyser::hashHeader(BlockHeader& header, unsigned char hash[])
{
    constexpr size_t bufsize = 4*sizeof(uint64_t) + 64*sizeof(uint8_t);
    unsigned char buf[bufsize];
    size_t offset = 0;

    // Copy everything into buf
    memcpy(&buf[offset], &header.index, sizeof(uint64_t));
    offset += sizeof(header.index);
    memcpy(&buf[offset], &header.version, sizeof(uint64_t));
    offset += sizeof(header.version);
    memcpy(&buf[offset], header.prevHash.data(), sizeof(header.prevHash));
    offset += sizeof(header.prevHash);
    memcpy(&buf[offset], header.merkleRoot.data(), sizeof(header.merkleRoot));
    offset += sizeof(header.merkleRoot);
    memcpy(&buf[offset], &header.time, sizeof(uint64_t));
    offset += sizeof(header.time);
    memcpy(&buf[offset], &header.nonce, sizeof(uint64_t));


    if (!SHA256(buf, bufsize, hash))
    {
        throw std::runtime_error("Failed to hash block");
    }
}


keyser::Block keyser::newBlock(uint64_t index, uint64_t time, unsigned char prevHash[], Transaction reward)
{
    Block block;
    block.index           = index;
    block.version         = KEYSER_VERSION;
    memcpy(prevHash, prevHash, SHA256_DIGEST_LENGTH);
    block.time            = time;
    block.nonce           = 0;

    // Body
    block.transactions.push_back(reward);

    return block;
}

keyser::BlockHeader keyser::getHeader(const Block& block)
{
    BlockHeader header;
    header.index    = block.index;
    header.version  = block.version;
    header.prevHash = block.prevHash;
    header.merkleRoot = block.merkleRoot;
    header.time     = block.time;
    header.nonce    = block.nonce;
    
    return header;
}

void keyser::calcMerkleRoot(std::vector<Transaction>& transactions, unsigned char merkleRoot[])
{
    uint8_t* buf;
    size_t offset = 0;

    for (int i = 0 ; i < transactions.size() ; i++)
    {
        unsigned char hash[SHA256_DIGEST_LENGTH];
        hashTransaction(transactions[i], hash);
    }

    if (!SHA256(buf, offset, merkleRoot))
    {
        throw std::runtime_error("Failed to calc merkleRoot");
    }
}
