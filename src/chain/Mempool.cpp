#include <iostream>
#include <vector>

#include "./Transaction.hpp"
#include "./Mempool.hpp"


void keyser::Mempool::addTransaction(Transaction transaction)
{
    // Validate transaction before adding it to Mempool
    if (transaction.isValid())
    {
        _pendingTransactions.push_back(transaction);
    }
}

std::vector<keyser::Transaction> keyser::Mempool::popLeadingTransactions()
{
    // In future, only allow a limited amount of transactions
    std::vector<keyser::Transaction> transactions = _pendingTransactions;
    
    _pendingTransactions.clear();

    return transactions;
}

void keyser::Mempool::printMempool()
{
    if (_pendingTransactions.size() == 0)
    {
        std::cout << "Mempool empty." << std::endl;
        return;
    }

    for (int i = 0 ; i < _pendingTransactions.size() ; i++)
    {
        std::cout << _pendingTransactions.at(i) << std::endl;
    }
}