#ifndef CHAIN_H
#define CHAIN_H

#include <vector>

#include "./Block.hpp"
#include "./Transaction.hpp"

namespace keyser 
{
    class Chain
    {   
    public:
        // Constructors
        Chain(uint8_t difficulty, uint8_t miningReward);

        // Accessors
        Block* getCurrBlock();

        // Modifiers
        void createTransaction(Transaction transaction);

        // Other
        void printChain();
        void createGenesisBlock();
        void mineBlock(std::string rewardAddress);
        void getAddressBalance(std::string address);
        
    private:
        Block*  _currBlock;
        uint8_t _difficulty;
        uint8_t _miningReward;

        std::vector<Transaction> _pendingTransactions;
    };
}

#endif