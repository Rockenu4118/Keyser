#include "./ValidationEngine.hpp"
#include "../node/Node.hpp"
#include "../chain/Block.hpp"
#include "../chain/Transaction.hpp"


keyser::ValidationEngine::ValidationEngine(Node* node) : _node(node)
{}

bool keyser::ValidationEngine::validateBlock(Block& block)
{
    // If so, convert to smart pointer and validate
    std::shared_ptr<Block> newBlock = std::make_shared<Block>(std::move(block));

    // TODO - will need more checks to ensure block is valid (difficulty, etc...)

    if (!newBlock->hasValidTransactions())
        return false;

    _node->blocks().push_back(newBlock);

    return true;
}

bool keyser::ValidationEngine::validateTransaction(Transaction transaction)
{
    // Make sure transaction hasn't already reached the mempool
    for (Transaction tx : _node->pendingTransactions())
    {
        if (tx.calcHash() == transaction.calcHash())
            return false;
    }
        
    // Validate transaction before adding it to Mempool
    if (!transaction.isValid())
        return false;

    _node->pendingTransactions().push_back(transaction);
    return true;
}