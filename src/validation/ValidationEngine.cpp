#include "./ValidationEngine.hpp"
#include "../node/Node.hpp"
#include "../chain/Block.hpp"
#include "../net/Connection.hpp"


keyser::ValidationEngine::ValidationEngine(Node& node) : _node(node)
{}

bool keyser::ValidationEngine::validateBlock(Block block)
{
    // Check is block is next up in the chain
    // if not, save block to map until parent block arrives
    if (_node.getCurrBlock()->_hash != block._prevHash)
    {
        _orphanBlocks[block._hash] = std::move(block);
        return false;
    }

    // If so, convert to smart pointer and validate
    std::shared_ptr<Block> newBlock = std::make_shared<Block>(std::move(block));

    // TODO - will need more checks to ensure block is valid (difficulty, etc...)

    if (!newBlock->hasValidTransactions())
        return false;

    _node.blocks().push_back(newBlock);

    // Check if stored orphan block can now be added
    for (auto& block : _orphanBlocks)
    {
        if (_node.getCurrBlock()->_hash == block.second._prevHash)
        {
            std::shared_ptr<Block> orphanBlock = std::make_shared<Block>(block.second);
            _node.blocks().push_back(orphanBlock);
            _orphanBlocks.erase(block.first);
        }
    }

    return true;
}