#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include <chrono>

#include "./Chain.hpp"
#include "./Block.hpp"


// Constructors
keyser::Chain::Chain(uint8_t difficulty, uint8_t miningReward)
{
    _currBlock  = nullptr;
    _difficulty = difficulty;
    _miningReward = miningReward;
    
    _pendingTransactions = std::vector<Transaction>{};
}

// Accessors
keyser::Block* keyser::Chain::getCurrBlock()
{
    return _currBlock;
}

// Modifiers
void keyser::Chain::createTransaction(keyser::Transaction transaction)
{
    _pendingTransactions.push_back(transaction);
}

void keyser::Chain::createGenesisBlock()
{
    keyser::Block* genesisBlock = new keyser::Block(0, nullptr, "None", _pendingTransactions);
    genesisBlock->calcHash();

    _currBlock = genesisBlock;
}

void keyser::Chain::mineBlock(std::string rewardAddress)
{
    keyser::Block* newBlock = new keyser::Block(_currBlock->getIndex() + 1, _currBlock, _currBlock->getHash(), _pendingTransactions);
    newBlock->calcValidHash(_difficulty);

    _currBlock = newBlock;

    _pendingTransactions.clear();

    _pendingTransactions.push_back(keyser::Transaction(_miningReward, rewardAddress, "None"));
}

// Other
void keyser::Chain::printChain()
{
    int count = 0;

    Block* temp = _currBlock;

    if (temp == NULL) 
    {
        std::cout << "\nChain empty." << std::endl;
        return; 
    }

    while (temp != NULL) 
    {
        std::cout << *temp << std::endl;
        temp = temp->getPrevBlock();
        count++;
    }

    std::cout << std::endl << "Block count: " << count << std::endl;
}

void keyser::Chain::getAddressBalance(std::string address)
{
    uint balance = 0;

    keyser::Block* temp = _currBlock;

    while (temp != NULL)
    {
        std::vector<Transaction> transactions = temp->getTransactions();

        for (uint i = 0 ; i < transactions.size() ; i++)
        {
            if (transactions.at(i).getRecieverAddress() == address)
                balance += transactions.at(i).getAmount();

            if (transactions.at(i).getSenderAddress() == address)
                balance -= transactions.at(i).getAmount();
        }
        temp = temp->getPrevBlock();
    }

    std::cout << std::endl;
    std::cout << "Address: " << address << ", Balance: " << balance << std::endl;
}

