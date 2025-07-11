#include <iostream>
#include <string>
#include <chrono>
#include <memory>

#include "../wallet/Wallet.hpp"
#include "./ChainIndex.hpp"

#include <openssl/sha.h>

#include "./Mempool.hpp"
#include "../data/keys.hpp"


keyser::ChainIndex::ChainIndex()
{
    createGenesisBlock();
}

void keyser::ChainIndex::createGenesisBlock()
{
    std::vector<Transaction> initialBalances{};

    // Transaction tx1(50000, addr1, 5);
    // tx1._time = 0;
    //
    // Block genesisBlock = Block(0, 0, "None", tx1, initialBalances);
    //
    // genesisBlock._time = 0;
    // genesisBlock._bodyHash = genesisBlock.bodyHash();

    // _node->validationEngine()->validateBlock(genesisBlock);
}

void keyser::ChainIndex::processBlock(Block& block)
{
    // std::array<uint8_t, SHA256_DIGEST_LENGTH> hash;
    // BlockHeader header = getHeader(block);
    // hashHeader(header, hash.data());

    // _blockIndex.insert(std::pair(block.index, hash));
    //
    // _headers.insert(std::pair(hash, header));
    //
    // _blocks.insert(std::pair(hash, block));
}

keyser::BlockIndex* keyser::ChainIndex::getTip()
{
    return mChain.at(getHeight());
}

// keyser::Block& keyser::Chain::getBlock(int index)
// {
//     std::string hash = _blockIndex.at(index);
//     return _blocks.at(hash);
// }
//
// keyser::Block& keyser::Chain::getBlock(std::string hash)
// {
//     return _blocks.at(hash);
// }

uint keyser::ChainIndex::calcDifficulty() const
{
    // TODO - dynamically calculate the difficulty based on amount of time
    // taken to mine previous blocks
    return 4;
}

uint keyser::ChainIndex::calcReward() const
{
    // TODO - dynamically calculate reward
    return 100;
}

uint32_t keyser::ChainIndex::getHeight() const
{
    return mChain.size() - 1;
}