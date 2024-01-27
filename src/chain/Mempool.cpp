#include <iostream>
#include <vector>

#include "./Transaction.hpp"
#include "./Mempool.hpp"


void keyser::Mempool::addTransaction(Transaction transaction)
{
    // Validate transaction before adding it to Mempool
    if (transaction.isValid())
    {
        _pendingTransactions.pushBack(transaction);
    }
}

std::vector<keyser::Transaction> keyser::Mempool::popLeadingTransactions()
{
    // In future, prioritize transactions by gas fee
    std::vector<keyser::Transaction> transactions;

    while (!_pendingTransactions.empty())
    {
        Transaction tx = _pendingTransactions.popFront();

        transactions.push_back(tx);
    }
    
    return transactions;
}

double keyser::Mempool::getPendingBalance(std::string address)
{
    double balance = 0;

    for (int i = 0 ; i < _pendingTransactions.count() ; i++)
    {
        if (_pendingTransactions.at(i)._recieverAddress == address)
            balance += _pendingTransactions.at(i)._amount;

        if (_pendingTransactions.at(i)._senderAddress == address)
            balance -= _pendingTransactions.at(i)._amount;
    }

    return balance;
}

void keyser::Mempool::printMempool()
{
    if (_pendingTransactions.count() == 0)
    {
        std::cout << "Mempool empty." << std::endl;
        return;
    }

    for (int i = 0 ; i < _pendingTransactions.count() ; i++)
    {
        Transaction t = _pendingTransactions.at(i);
        std::cout << t << std::endl;
    }
}