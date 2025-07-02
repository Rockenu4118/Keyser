#ifndef MESSAGE_H
#define MESSAGE_H

#include <vector>

#include "../chain/Transaction.hpp"



namespace keyser
{
    struct NetMessageHeader
    {
        uint8_t magic;
        char command[16];
        size_t size = 0;
    };

    struct NetMessage
    {
        NetMessageHeader header;
        std::vector<unsigned char> body;
    };
}

#endif