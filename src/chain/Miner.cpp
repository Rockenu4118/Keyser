#include "./Miner.hpp"


keyser::Miner::Miner(Node* node) : _node(node)
{}

keyser::Miner::~Miner()
{
    stop();
}

void keyser::Miner::start(uint numBlocks)
{   
    if (_minerActive)
    {
        std::cout << "[CHAIN] Mining In Progress" << std::endl;
        return;
    }

    if (_miningThr.joinable())
        _miningThr.join();

    _miningThr = std::thread([this, numBlocks]()
    {
        _minerActive = true;

        uint blocksMined = 0;

        while (!_node->shutdownFlag() && _minerActive && (blocksMined < numBlocks))
        {
            std::shared_ptr<Block> newBlock = constructBlock("0xc6d8a2c830495d07318212e9f2cad16f");

            while (1)
            {
                if (_node->shutdownFlag() || !_minerActive)
                    return;

                if (utils::isValidHash(newBlock->hash(), _node->chain()->calcDifficulty()))
                    break;

                newBlock->_nonce++;
            }

            blocksMined++;

            std::cout << "[CHAIN] Block Mined." << std::endl;

            if (_node->validationEngine()->validateBlock(newBlock))
                _node->network()->distributeBlock(*_node->chain()->getCurrBlock());
        }

        _minerActive = false;
    });
}

void keyser::Miner::stop()
{
    _minerActive = false;

    if (_miningThr.joinable())
        _miningThr.join();
}

std::shared_ptr<keyser::Block> keyser::Miner::constructBlock(std::string rewardRecipient)
{
    auto block = std::make_shared<Block>(_node->chain()->getCurrBlock()->_index + 1, time(NULL), _node->chain()->getCurrBlock()->hash(), _node->mempool()->leadingTransactions());

    Transaction blockReward(_node->chain()->calcReward(), rewardRecipient);

    block->_transactions.push_back(blockReward);

    block->_bodyHash = block->bodyHash();

    return block;
}