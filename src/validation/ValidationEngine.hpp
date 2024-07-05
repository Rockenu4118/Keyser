#ifndef VALIDATION_ENGINE_H
#define VALIDATION_ENGINE_H

#include <vector>
#include <memory>
#include <string>
#include <thread>
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
            ValidationEngine(Node* node);

            ~ValidationEngine() = default;

            bool validateBlock(std::shared_ptr<Block> block);

            void processValidBlock(std::shared_ptr<Block> block);

            bool validateTransaction(Transaction transaction);

        private:
            Node* _node;
    };
}

#endif