#ifndef MSG_TYPES_H
#define MSG_TYPES_H

enum class MsgTypes : uint32_t {
    // Client Origin
    Ping,
    Version,
    MessageAll,

    // Server Origin
    Transaction,
    Block,
    ServerMessage,
    ServerAccept
};

#endif