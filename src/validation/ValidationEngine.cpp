#include <algorithm>

#include "./ValidationEngine.hpp"


keyser::ValidationEngine::ValidationEngine(Node* node) : _node(node)
{}

bool keyser::ValidationEngine::validateBlock(std::shared_ptr<Block> block)
{
    // TODO - validate block transactions and hash difficulty

    // Make sure there is only one block reward included
    if (!(std::count_if(block->_transactions.begin(), block->_transactions.end(), [](Transaction tx){ return tx._inputs.size() == 0; }) == 1))
    {
        std::cout << "Not valid block" << std::endl;
        return false;
    }

    processValidBlock(block);

    return true;
}


void keyser::ValidationEngine::processValidBlock(std::shared_ptr<Block> block)
{
    // Update mempool
    _node->mempool()->processBlock(block);

    // Update utxo set
    _node->utxoSet()->processBlock(block);

    // Add block to chain
    _node->chain()->blocks().push_back(block);
}

bool keyser::ValidationEngine::validateTransaction(Transaction transaction)
{
    // Make sure transaction hasn't already reached the mempool
    auto iter = _node->mempool()->pendingTransactions().find(transaction.hash());

    if (iter != _node->mempool()->pendingTransactions().end())
        return false;

    // Make sure transaction is signed
    if (!transaction.isSigned())
        return false;

    // Validate inputs
    for (auto input : transaction._inputs)
    {
        if (!_node->utxoSet()->isUtxoPresent(input._outPoint.calcHash()))
            return false;

        UTXO utxo = _node->utxoSet()->utxoSet()[input._outPoint.calcHash()];

        cryptography::ECKeyPair keyPair = cryptography::ECKeyPair("public", input._pubKey);

        if (!keyPair.verify(transaction.hash(), input._sig._rSigVal, input._sig._sSigVal))
            return false;

        if (utils::pubKeytoAddress(input._pubKey) != utxo._output._recipient)
            return false;
    }

    // Add to mempool
    _node->mempool()->pendingTransactions()[transaction.hash()] = transaction;

    std::cout << "[CHAIN] Valid Tx!" << std::endl;

    return true;
}