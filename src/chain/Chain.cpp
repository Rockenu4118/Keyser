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

    Transaction tx1(50000, addr1);
    tx1._time = 0;

    initialBalances.push_back(tx1);

    auto genesisBlock = std::make_shared<Block>(0, 0, "None", initialBalances);

    genesisBlock->_time = 0;
    genesisBlock->_bodyHash = genesisBlock->bodyHash();

    _node->validationEngine()->validateBlock(genesisBlock);
}

std::shared_ptr<keyser::Block> keyser::Chain::getCurrBlock()
{
    return _blocks.back();
}

std::shared_ptr<keyser::Block> keyser::Chain::getBlock(int index)
{
    return _blocks.at(index);
}

uint keyser::Chain::calcDifficulty()
{
    // TODO - dynamically calculate the difficulty based on amount of time
    // taken to mine previous blocks
    return 4;
}

uint keyser::Chain::calcReward()
{
    // TODO - dynamically calculate reward
    return 100;
}

void keyser::Chain::printChain()
{
    if (_blocks.size() == 0)
    {
        std::cout << "Chain empty." << std::endl;
        return;
    }

    for (int i = 0 ; i < _blocks.size() ; i++)
    {
        std::cout << *_blocks.at(i) << std::endl;
    }
}

bool keyser::Chain::isValid()
{
    for (int i = 1 ; i < _blocks.size() ; i++)
    {
        std::shared_ptr<Block> currBlock = _blocks.at(i);
        std::shared_ptr<Block> prevBlock = _blocks.at(i - 1);

        if (prevBlock->hash() != currBlock->_prevHash)
            return false;

        if (!currBlock->hasValidTransactions())
            return false;
    }

    return true;
}

uint keyser::Chain::getHeight() const
{
    return _blocks.size();
}

bool& keyser::Chain::blockInvRecieved()
{
    return _blockInvRecieved;
}

std::vector<int>& keyser::Chain::inventory()
{
    return _inventory;
}

std::vector<std::shared_ptr<keyser::Block>>& keyser::Chain::blocks()
{
    return _blocks;
}