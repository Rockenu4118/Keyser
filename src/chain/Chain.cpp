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
keyser::Chain::Chain(uint8_t difficulty, uint8_t miningReward)
{
    _difficulty = difficulty;
    _miningReward = miningReward;

    _mempool = new Mempool();

    createGenesisBlock();
}

// Accessors
std::shared_ptr<keyser::Block> keyser::Chain::getCurrBlock()
{
    return _blocks.back();
}

// Modifiers
void keyser::Chain::createGenesisBlock()
{
    std::shared_ptr<Block> genesisBlock = std::make_shared<Block>(0, 0, "None", _mempool->popLeadingTransactions());

    genesisBlock->calcHash();

    _blocks.push_back(genesisBlock);
}

void keyser::Chain::mineBlock(std::string rewardAddress)
{
    std::shared_ptr<Block> newBlock = std::make_shared<Block>(getCurrBlock()->_index + 1, time(NULL), getCurrBlock()->_hash, _mempool->popLeadingTransactions());

    newBlock->calcValidHash(_difficulty);

    _blocks.push_back(newBlock);

    _mempool->addTransaction(keyser::Transaction(_miningReward, rewardAddress, "None"));
}

void keyser::Chain::addBlock(Block block)
{
    std::shared_ptr<Block> newBlock = std::make_shared<Block>(block);
    
    if (newBlock->hasValidTransactions())
        _blocks.push_back(newBlock);
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

void keyser::Chain::getAddressBalance(std::string address)
{
    double balance = 0;

    for (int i = 0 ; i < _blocks.size() ; i++)
    {
        std::vector<Transaction> transactions = _blocks.at(i)->_transactions;

        for (int j = 0 ; j < transactions.size() ; j++)
        {
            if (transactions.at(j)._recieverAddress == address)
                balance += transactions.at(j)._amount;

            if (transactions.at(j)._senderAddress == address)
                balance -= transactions.at(j)._amount;
        }
    }
    
    std::cout << "Address: " << address << ", Balance: " << balance << std::endl;
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

keyser::Mempool* keyser::Chain::mempool()
{
    return _mempool;
}