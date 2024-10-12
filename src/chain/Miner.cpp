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
            Block newBlock = constructBlock("0xc6d8a2c830495d07318212e9f2cad16f");

            while (1)
            {
                if (_node->shutdownFlag() || !_minerActive)
                    return;

                if (utils::isValidHash(newBlock.hash(), _node->chain()->calcDifficulty()))
                    break;

                newBlock._nonce++;
            }

            blocksMined++;

            std::cout << "[CHAIN] Block Mined." << std::endl;

            if (_node->validationEngine()->validateBlock(newBlock))
                _node->network()->distributeBlock(newBlock);
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

keyser::Block keyser::Miner::constructBlock(std::string rewardRecipient)
{
    int currBlockI = _node->chain()->getCurrBlock()._index;

    Transaction blockReward(_node->chain()->calcReward(), rewardRecipient, currBlockI + 6);

    Block block = Block(currBlockI + 1, time(nullptr), _node->chain()->getCurrBlock().hash(), blockReward, _node->mempool()->leadingTransactions());
    
    block._bodyHash = block.bodyHash();

    return block;
}