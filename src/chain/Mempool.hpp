#ifndef MEMPOOL_H
#define MEMPOOL_H

#include <string>
#include <unordered_map>

#include "../node/Node.hpp"
#include "./Transaction.hpp"

namespace keyser
{
    class Node;

    class Mempool
    {
        public:
            Mempool(Node* node);

            std::vector<Transaction> leadingTransactions();

            void processBlock(Block& block);

            void printMempool() const;

            std::unordered_map<std::string, Transaction>& pendingTransactions();

        private:
            Node* _node;

            std::unordered_map<std::string, Transaction> _pendingTransactions;
    };
}

#endif