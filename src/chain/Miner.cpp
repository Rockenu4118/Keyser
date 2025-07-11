#include "./Miner.hpp"

#include "../app/App.hpp"


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

        while (!mShutdown && _minerActive && (blocksMined < numBlocks))
        {
            Transaction reward{};

            // Block block = newBlock(_node->chain()->getTip().index + 1, time(nullptr), nullptr, reward);

            while (true)
            {
                if (mShutdown || !_minerActive)
                    return;

                // if (isValidHash(newBlock.hash(), _node->chain()->calcDifficulty()))
                //     break;

                // block.nonce++;
            }

            blocksMined++;

            std::cout << "[CHAIN] Block Mined." << std::endl;

            // if (_node->validationEngine()->validateBlock(newBlock))
            //     _node->network()->distributeBlock(newBlock);
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

bool keyser::Miner::isValidHash(std::string hash, uint8_t difficulty)
{
    for (uint i = 0 ; i < difficulty ; i++)
    {
        if (hash[i] != '0')
        {
            return false;
        }
    }

    return true;
}