#ifndef VALIDATION_ENGINE_H
#define VALIDATION_ENGINE_H

#include <vector>
#include <string>
#include <unordered_map>

#include "../chain/Block.hpp"
#include "../chain/Transaction.hpp"
#include "../node/Node.hpp"


namespace keyser
{   
    // Forward declare
    class Node;

    class ValidationEngine
    {
        public:
            ValidationEngine(Node& node);

            bool validateBlock(Block block);

            bool validateTransaction(Transaction transaction);

        private:
            Node& _node;

            std::unordered_map<std::string, Block> _orphanBlocks{};
  
    };
}

#endif