#include <iostream>
#include <vector>

#include "./Transaction.hpp"
#include "./Mempool.hpp"


bool keyser::Mempool::addTransaction(Transaction transaction)
{
    // Make sure transaction hasn't already reached the mempool
    for (Transaction tx : _pendingTransactions)
    {
        if (tx._hash == transaction._hash)
            return false;
    }
        
    // Validate transaction before adding it to Mempool
    if (!transaction.isValid())
        return false;

    _pendingTransactions.push_back(transaction);
    return true;
}

std::vector<keyser::Transaction> keyser::Mempool::popLeadingTransactions()
{
    // TODO - priortize gas fees
    std::vector<keyser::Transaction> transactions;

    for (Transaction tx : _pendingTransactions)
    {
        transactions.push_back(tx);
        _pendingTransactions.pop_front();
    }
    
    return transactions;
}

double keyser::Mempool::getPendingBalance(std::string address)
{
    double balance = 0;

    for (Transaction tx : _pendingTransactions)
    {
        if (tx._recieverAddress == address)
            balance += tx._amount;

        if (tx._senderAddress == address)
            balance -= tx._amount;
    }

    return balance;
}

void keyser::Mempool::printMempool()
{
    if (_pendingTransactions.size() == 0)
    {
        std::cout << "Mempool empty." << std::endl;
        return;
    }

    for (Transaction tx : _pendingTransactions)
        std::cout << tx << std::endl;
}