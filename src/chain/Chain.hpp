#ifndef CHAIN_H
#define CHAIN_H

#include <vector>
#include <memory>

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
        std::shared_ptr<Block> getCurrBlock();

        void createTransaction(Transaction transaction);

        void printChain();
        void createGenesisBlock();
        void mineBlock(std::string rewardAddress);
        void addBlock(Block block);
        void getAddressBalance(std::string address);
        
    private:
        std::vector<std::shared_ptr<Block>> _blocks;
        std::vector<Transaction>            _pendingTransactions;
        
        uint8_t _difficulty;
        uint8_t _miningReward;
    };
}

#endif