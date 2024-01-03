#ifndef MEMPOOL_H
#define MEMPOOL_H

#include <tsqueue.hpp>

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
            tsqueue<Transaction> _pendingTransactions;

    };
}

#endif