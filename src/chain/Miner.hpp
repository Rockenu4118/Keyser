#ifndef MINER_H
#define MINER_H

#include <thread>

#include "../node/Node.hpp"


namespace keyser
{
    class Node;

    class Miner
    {
        public:
            explicit Miner(Node* node);

            ~Miner();

            void start(uint numBlocks = -1);

            void stop();

            Block constructBlock(std::string rewardRecipient);

        private:
            Node* _node;

            std::thread _miningThr;

            bool _minerActive = false;
    };
}

#endif