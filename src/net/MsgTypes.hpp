#ifndef MSG_TYPES_H
#define MSG_TYPES_H

enum class MsgTypes : uint32_t 
{
    MinedBlock,
    Transaction,
    Ping,
    Version,
    InitBlockDownload,
    Block,
};

#endif