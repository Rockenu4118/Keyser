#include <iostream>
#include <vector>

#include "./Mempool.hpp"


// std::vector<keyser::Transaction> keyser::Mempool::leadingTransactions()
// {
//     // TODO - priortize gas fees
//     std::vector<keyser::Transaction> transactions;
//
//     for (auto tx : _pendingTransactions)
//     {
//         transactions.push_back(tx.second);
//     }
//
//     return transactions;
// }
//
// void keyser::Mempool::processBlock(Block& block)
// {
//     // for (auto tx : block._transactions)
//     // {
//     //     auto iter = _pendingTransactions.find(tx.hash());
//     //
//     //     if (iter != _pendingTransactions.end())
//     //         _pendingTransactions.erase(iter);
//     // }
// }