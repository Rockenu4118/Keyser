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
            Chain(uint8_t difficulty, uint8_t miningReward);

            std::shared_ptr<Block> getCurrBlock();
            std::shared_ptr<Block> getBlock(int index);

            void   printChain();
            void   createGenesisBlock();
            void   mineBlock(std::string rewardAddress);
            void   addBlock(Block block);
            double getAddressBalance(std::string address);
            bool   isValid();

            std::vector<std::shared_ptr<Block>>& blocks();
            Mempool* mempool();
        
        private:
            std::vector<std::shared_ptr<Block>> _blocks;
            Mempool*                            _mempool = nullptr;
        
            uint8_t _difficulty;
            uint8_t _miningReward;
    };
}

#endif