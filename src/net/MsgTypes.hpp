#ifndef MSG_TYPES_H
#define MSG_TYPES_H

enum class MsgTypes : uint32_t {
    Ping,
    Version,
    MessageAll,
    Transaction,
    Block,
    ServerMessage,
    ServerAccept
};

#endif