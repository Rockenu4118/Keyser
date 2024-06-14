#ifndef MEMPOOL_H
#define MEMPOOL_H

#include <deque>

#include "./Transaction.hpp"

namespace keyser
{
    class Mempool
    {
        public:
            Mempool() = default;

            std::vector<Transaction> popLeadingTransactions();

            // double getPendingBalance(std::string address);

            void printMempool();

            std::deque<Transaction>& pendingTransactions();

        protected:
            std::deque<Transaction> _pendingTransactions;

    };
}

#endif