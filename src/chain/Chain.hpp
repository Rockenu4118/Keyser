#ifndef CHAIN_H
#define CHAIN_H

#include <vector>
#include <memory>

#include "./Block.hpp"
#include "./Transaction.hpp"
#include "./Mempool.hpp"

namespace keyser 
{
    class Chain
    {   
    public:
        // Constructors
        Chain(uint8_t difficulty, uint8_t miningReward);

        // Accessors
        std::shared_ptr<Block> getCurrBlock();

        void printChain();
        void createGenesisBlock();
        void mineBlock(std::string rewardAddress);
        void addBlock(Block block);
        void getAddressBalance(std::string address);
        bool isValid();

        Mempool* mempool();
        
    private:
        std::vector<std::shared_ptr<Block>> _blocks;
        Mempool*                            _mempool;
        
        uint8_t _difficulty;
        uint8_t _miningReward;
    };
}

#endif