// TODO - potentially create a structure that will be responsible for
// mining and contain a mining thread. This structure will be dynamically called
// whenever a using wants to begin mining, and destroyed when they are finished.

#ifndef MINER_H
#define MINER_H

#include <memory>
#include <thread>

#include "../node/Node.hpp"


namespace keyser
{
    class Node;

    class Miner
    {
        public:
            Miner(Node* node);

            ~Miner();

            void start(uint numBlocks = -1);

            void stop();

            std::shared_ptr<Block> constructBlock(std::string rewardRecipient);

        private:
            Node* _node;

            std::thread _miningThr;

            bool _minerActive = false;
    };
}

#endif