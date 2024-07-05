#include <iostream>
#include <vector>

#include "./Mempool.hpp"


keyser::Mempool::Mempool(Node* node) : _node(node)
{}

std::vector<keyser::Transaction> keyser::Mempool::leadingTransactions()
{
    // TODO - priortize gas fees
    std::vector<keyser::Transaction> transactions;

    for (auto tx : _pendingTransactions)
    {
        transactions.push_back(tx.second);
    }
    
    return transactions;
}

void keyser::Mempool::processBlock(std::shared_ptr<Block> block)
{
    for (auto tx : block->_transactions)
    {
        auto iter = _pendingTransactions.find(tx.hash());

        if (iter != _pendingTransactions.end())
            _pendingTransactions.erase(iter);
    }
}

void keyser::Mempool::printMempool() const
{
    if (_pendingTransactions.size() == 0)
    {
        std::cout << "Mempool empty." << std::endl;
        return;
    }

    for (auto tx : _pendingTransactions)
        std::cout << tx.second << std::endl;
}

std::unordered_map<std::string, keyser::Transaction>& keyser::Mempool::pendingTransactions()
{
    return _pendingTransactions;
}