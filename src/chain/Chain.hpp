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
            Chain();

            std::shared_ptr<Block> getCurrBlock();
            std::shared_ptr<Block> getBlock(int index);

            void   printChain();
            void   createGenesisBlock();
            uint   calcDifficulty();
            bool   addBlock(Block block);
            double getAddressBalance(std::string address);
            bool   isValid();
            uint   getHeight() const;

            std::vector<std::shared_ptr<Block>>& blocks();
        
        protected:
            std::vector<std::shared_ptr<Block>> _blocks;
        
    };
}

#endif