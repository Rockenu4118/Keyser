#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include <chrono>

#include "./include/Chain.hpp"
#include "./include/Block.hpp"
#include "./include/util.hpp"


// Constructors
Keyser::Chain::Chain(uint8_t difficulty, uint8_t miningReward)
{
    _currBlock  = NULL;
    _difficulty = difficulty;
    _miningReward = miningReward;

    createGenesisBlock();
}

// Accessors
Keyser::Block* Keyser::Chain::getCurrBlock()
{
    return _currBlock;
}

// Modifiers
void Keyser::Chain::createTransaction(Keyser::Transaction transaction)
{
    _pendingTransactions.push_back(transaction);
}

void Keyser::Chain::createGenesisBlock()
{
    Keyser::Block* genesisBlock = new Keyser::Block(0, NULL, "None", _pendingTransactions);
    genesisBlock->calcHash();

    _currBlock = genesisBlock;
}

void Keyser::Chain::mineBlock(std::string rewardAddress)
{
    Keyser::Block* newBlock = new Keyser::Block(_currBlock->getIndex() + 1, _currBlock, _currBlock->getHash(), _pendingTransactions);
    newBlock->calcValidHash(_difficulty);

    _currBlock = newBlock;

    _pendingTransactions.clear();

    _pendingTransactions.push_back(Keyser::Transaction(100, "None", "theguy"));
}

// Other
void Keyser::Chain::printChain()
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
        temp->printBlock();
        temp = temp->getPrevBlock();
        count++;
    }

    std::cout << std::endl << "Block count: " << count << std::endl;
}

void Keyser::Chain::getAddressBalance(std::string address)
{
    uint balance = 0;

    Keyser::Block* temp = _currBlock;

    while (temp != NULL)
    {
        std::vector<Transaction> transactions = temp->getTransactions();

        for (uint i = 0 ; i < transactions.size() ; i++)
        {
            if (transactions.at(i).getPayee() == address)
                balance += transactions.at(i).getAmount();

            if (transactions.at(i).getPayer() == address)
                balance -= transactions.at(i).getAmount();
        }
        temp = temp->getPrevBlock();
    }

    std::cout << std::endl;
    std::cout << "Address: " << address << ", Balance: " << balance << std::endl;
}

