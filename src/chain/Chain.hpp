#ifndef CHAIN_H
#define CHAIN_H

#include <vector>
#include <memory>
#include <thread>
#include <list>
#include <unordered_map>

#include "../node/Node.hpp"
#include "./Block.hpp"
#include "./Transaction.hpp"
#include "./Mempool.hpp"

namespace keyser 
{
    class Node;

    class Chain
    {   
        public:
            Chain(Node* node);
            
            ~Chain() = default;

            // Initialize the one hardcoded block and add it to the chain
            void createGenesisBlock();

            std::shared_ptr<Block> getCurrBlock();
            std::shared_ptr<Block> getBlock(int index);

            void   printChain();
            
            uint   calcDifficulty();
            uint   calcReward();
            bool   isValid();
            uint   getHeight() const;
            
            bool& blockInvRecieved();
            std::vector<int>& inventory();
            std::vector<std::shared_ptr<Block>>& blocks();
        
        private:
            Node* _node;

            bool _blockInvRecieved = false;

            // Block indexes needed to complete local chain
            std::vector<int> _inventory;


            // std::unordered_map<uint, std::string> _chainIndex;

            // Container of pointers to block objects
            // std::unordered_map<std::string, Block> _blocks;
            std::vector<std::shared_ptr<Block>> _blocks;
    };
}

#endif