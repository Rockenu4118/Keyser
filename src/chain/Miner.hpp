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
            ~Miner();

            void start(uint numBlocks = -1);

            void stop();

            static bool isValidHash(std::string hash, uint8_t difficulty);

            std::thread _miningThr;

            bool _minerActive = false;
    };
}

#endif