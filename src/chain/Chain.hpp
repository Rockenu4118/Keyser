#ifndef CHAIN_H
#define CHAIN_H

#include <vector>
#include <memory>
#include <thread>
#include <list>
#include <unordered_map>
#include <map>

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
            explicit Chain(Node* node);
            
            ~Chain() = default;

            // Initialize the one hardcoded block and add it to the chain
            void createGenesisBlock();

            void processBlock(Block& block);

            Block& getCurrBlock();
            Block& getBlock(int index);
            Block& getBlock(std::string hash);

            void printChain();

            void printHeaders();
            
            uint calcDifficulty() const;
            uint calcReward() const;
            bool isValid() const;
            uint getHeight() const;
            
            std::vector<std::string>& inventory();

            std::map<int, std::string>& blockIndex();

            std::unordered_map<std::string, BlockHeader>& headers();

            std::unordered_map<std::string, Block>& blocks();
        
        private:
            Node* _node;

            // bool _blockInvRecieved = false;

            // Block indexes needed to complete local chain
            std::vector<std::string> _inventory;

            std::map<int, std::string> _blockIndex;

            std::unordered_map<std::string, BlockHeader> _headers;

            std::unordered_map<std::string, Block> _blocks;


            // std::unordered_map<uint, std::string> _chainIndex;

            // Container of pointers to block objects
            // std::unordered_map<std::string, Block> _blocks;
            // std::vector<std::shared_ptr<Block>> _blocks;
    };
}

#endif