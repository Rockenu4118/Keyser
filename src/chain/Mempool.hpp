#ifndef MEMPOOL_H
#define MEMPOOL_H

#include <vector>

#include "./Transaction.hpp"

namespace keyser
{
    class Mempool
    {
        public:
            Mempool() = default;

            void addTransaction(Transaction transaction);

            std::vector<Transaction> popLeadingTransactions();

            void printMempool();

        private:
            std::vector<Transaction> _pendingTransactions;

    };
}

#endif