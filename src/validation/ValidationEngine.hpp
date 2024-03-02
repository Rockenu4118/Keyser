#ifndef VALIDATION_ENGINE_H
#define VALIDATION_ENGINE_H

#include <vector>
#include <string>
#include <unordered_map>

#include "../chain/Block.hpp"
#include "../net/Connection.hpp"
#include "../node/Node.hpp"


namespace keyser
{   
    // Forward declare
    class Node;

    class ValidationEngine
    {
        public:
            // ValidationEngine() = default;
            ValidationEngine(Node& node);

            bool validateBlock(Block block);

            bool validateTransaction();

        private:
            Node& _node;

            std::unordered_map<std::string, Block> _orphanBlocks{};
  
    };
}

#endif