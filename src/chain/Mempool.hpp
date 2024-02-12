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

            bool addTransaction(Transaction transaction);

            std::vector<Transaction> popLeadingTransactions();

            double getPendingBalance(std::string address);

            void printMempool();

        private:
            std::deque<Transaction> _pendingTransactions;

    };
}

#endif