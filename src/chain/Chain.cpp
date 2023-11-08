#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include <chrono>


#include "./Chain.hpp"
#include "./Block.hpp"


// Constructors
keyser::Chain::Chain(uint8_t difficulty, uint8_t miningReward)
{
    _difficulty = difficulty;
    _miningReward = miningReward;

    createGenesisBlock();
}

// Accessors
std::shared_ptr<keyser::Block> keyser::Chain::getCurrBlock()
{
    return _blocks.back();
}

// Modifiers
void keyser::Chain::createTransaction(keyser::Transaction transaction)
{
    _pendingTransactions.push_back(transaction);
}

void keyser::Chain::createGenesisBlock()
{
    std::shared_ptr<Block> genesisBlock = std::make_shared<Block>(0, 0, "None", _pendingTransactions);

    genesisBlock->calcHash();

    _blocks.push_back(genesisBlock);
}

void keyser::Chain::mineBlock(std::string rewardAddress)
{
    std::shared_ptr<Block> newBlock = std::make_shared<Block>(getCurrBlock()->_index + 1, time(NULL), getCurrBlock()->_hash, _pendingTransactions);

    newBlock->calcValidHash(_difficulty);

    _blocks.push_back(newBlock);

    _pendingTransactions.clear();

    _pendingTransactions.push_back(keyser::Transaction(_miningReward, rewardAddress, "None"));
}

void keyser::Chain::addBlock(Block block)
{
    std::shared_ptr<Block> newBlock = std::make_shared<Block>(block);
    _blocks.push_back(newBlock);
}

// Other
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

