#ifndef CHAIN_H
#define CHAIN_H

#include <unordered_map>
#include <vector>
#include <memory>
#include <thread>
#include <list>

#include "./Block.hpp"
#include "./Transaction.hpp"
#include "./Mempool.hpp"

namespace keyser 
{
    class Chain
    {   
        public:
            Chain();

            // Initialize the one hardcoded block and add it to the chain
            void createGenesisBlock();

            std::shared_ptr<Block> getCurrBlock();
            std::shared_ptr<Block> getBlock(int index);

            void   printChain();
            
            uint   calcDifficulty();
            double getAddressBalance(std::string address);
            bool   isValid();
            uint   getHeight() const;

            std::vector<int> inventory();
            std::vector<std::shared_ptr<Block>>& blocks();
        
        protected:
            bool _blockInvRecieved = false;

            // Block indexes needed to complete local chain
            std::vector<int> _inventory;

            // Container of pointers to block objects
            std::vector<std::shared_ptr<Block>> _blocks;
        
    };
}

#endif