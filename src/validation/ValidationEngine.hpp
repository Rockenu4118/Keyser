#ifndef VALIDATION_ENGINE_H
#define VALIDATION_ENGINE_H

#include <vector>
#include <memory>
#include <string>
#include <thread>
#include <algorithm>
#include <unordered_map>
#include <keycrypto/Stealth.hpp>

#include "../primitives/Block.hpp"
#include "../primitives/Transaction.hpp"
#include "../node/Node.hpp"


// namespace keyser
// {
//     // Forward declare
//     class Node;
//
//     class ValidationEngine
//     {
//         public:
//             ValidationEngine(Node* node);
//
//             ~ValidationEngine() = default;
//
//             bool validateBlock(Block& block);
//
//             void processValidBlock(Block& block);
//
//             bool validateHeader(BlockHeader header);
//
//             void processValidHeader(BlockHeader header);
//
//             bool validateTransaction(Transaction transaction);
//
//         private:
//             Node* _node;
//     };
// }

#endif