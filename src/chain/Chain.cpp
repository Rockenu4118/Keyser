#include <iostream>
#include <string>
#include <stdlib.h>
#include <iomanip>
#include <chrono>
#include <memory>


#include "./Chain.hpp"
#include "./Block.hpp"
#include "./Mempool.hpp"


// Constructors
keyser::Chain::Chain()
{
    createGenesisBlock();
}

// Accessors
std::shared_ptr<keyser::Block> keyser::Chain::getCurrBlock()
{
    return _blocks.back();
}

std::shared_ptr<keyser::Block> keyser::Chain::getBlock(int index)
{
    return _blocks.at(index);
}

void keyser::Chain::createGenesisBlock()
{
    std::shared_ptr<Block> genesisBlock = std::make_shared<Block>(0, 0, "None", 100000, "genesis");

    genesisBlock->calcHash();

    _blocks.push_back(genesisBlock);
}

uint keyser::Chain::calcDifficulty()
{
    // TODO - dynamically calculate the difficulty based on amount of time
    // taken to mine previous blocks
    return 4;
}

bool keyser::Chain::addBlock(Block block)
{
    std::shared_ptr<Block> newBlock = std::make_shared<Block>(std::move(block));
    
    // TODO - will need more checks to ensure block is valid
    if (!newBlock->hasValidTransactions())
        return false;

    if (_blocks.back()->_hash != newBlock->_prevHash)
        return false;

    _blocks.push_back(newBlock);
    return true;
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

double keyser::Chain::getAddressBalance(std::string address)
{
    double balance = 0;

    for (int i = 0 ; i < _blocks.size() ; i++)
    {
        // Modify balances from genesis block and mining rewards
        if (_blocks.at(i)->_rewardAddress == address)
            balance += _blocks.at(i)->_reward;

        std::vector<Transaction> transactions = _blocks.at(i)->_transactions;

        // Modify balance from transactions
        for (int j = 0 ; j < transactions.size() ; j++)
        {
            if (transactions.at(j)._recieverAddress == address)
                balance += transactions.at(j)._amount;

            if (transactions.at(j)._senderAddress == address)
                balance -= transactions.at(j)._amount;
        }
    }
    
    return balance;
}

bool keyser::Chain::isValid()
{
    for (int i = 1 ; i < _blocks.size() ; i++)
    {
        std::shared_ptr<Block> currBlock = _blocks.at(i);
        std::shared_ptr<Block> prevBlock = _blocks.at(i - 1);

        if ((*prevBlock)._hash != (*currBlock)._prevHash)
            return false;

        if (!currBlock->hasValidTransactions())
            return false;

        std::string currBlockHash = (*currBlock)._hash;
        currBlock->calcHash();

        if ((*currBlock)._hash != currBlockHash)
            return false;
    }

    return true;
}

uint keyser::Chain::getHeight() const
{
    return _blocks.size();
}

std::vector<std::shared_ptr<keyser::Block>>& keyser::Chain::blocks()
{
    return _blocks;
}