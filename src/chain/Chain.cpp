#include <iostream>
#include <string>
#include <stdlib.h>
#include <iomanip>
#include <chrono>
#include <memory>

#include "../wallet/Wallet.hpp"
#include "./Chain.hpp"
#include "./Mempool.hpp"
#include "../data/keys.hpp"


keyser::Chain::Chain(Node* node) : _node(node)
{}

void keyser::Chain::createGenesisBlock()
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

void keyser::Chain::processBlock(Block& block)
{
    // _blockIndex.insert(std::pair(block._index, block.hash()));
    //
    // _headers.insert(std::pair(block.hash(), block.getHeader()));
    //
    // _blocks.insert(std::pair(block.hash(), block));
}

keyser::Block& keyser::Chain::getCurrBlock()
{
    std::string hash = std::prev(_blockIndex.end())->second;
    return _blocks.at(hash);
}

keyser::Block& keyser::Chain::getBlock(int index)
{
    std::string hash = _blockIndex.at(index);
    return _blocks.at(hash);
}

keyser::Block& keyser::Chain::getBlock(std::string hash)
{
    return _blocks.at(hash);
}

uint keyser::Chain::calcDifficulty() const
{
    // TODO - dynamically calculate the difficulty based on amount of time
    // taken to mine previous blocks
    return 4;
}

uint keyser::Chain::calcReward() const
{
    // TODO - dynamically calculate reward
    return 100;
}

// void keyser::Chain::printChain()
// {
//     if (_blocks.size() == 0)
//     {
//         std::cout << "Chain empty." << std::endl;
//         return;
//     }
//
//     for (const auto&[index, hash] : _blockIndex)
//     {
//         std::cout << _blocks.at(hash) << std::endl;
//     }
// }
//
// void keyser::Chain::printHeaders()
// {
//     if (_headers.size() == 0)
//     {
//         std::cout << "Headers empty." << std::endl;
//         return;
//     }
//
//     for (const auto&[index, hash] : _blockIndex)
//     {
//         std::cout << _headers.at(hash) << std::endl;
//     }
// }

bool keyser::Chain::isValid() const
{
    // TODO - make chain isValid func
    return true;
}

uint keyser::Chain::getHeight() const
{
    return _blocks.size();
}

std::vector<std::string>& keyser::Chain::inventory()
{
    return _inventory;
}

std::map<int, std::string>& keyser::Chain::blockIndex()
{
    return _blockIndex;
}

std::unordered_map<std::string, keyser::BlockHeader>& keyser::Chain::headers()
{
    return _headers;
}

std::unordered_map<std::string, keyser::Block>& keyser::Chain::blocks()
{
    return _blocks;
}