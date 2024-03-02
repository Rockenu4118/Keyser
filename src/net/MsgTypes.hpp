#ifndef MSG_TYPES_H
#define MSG_TYPES_H

enum class MsgTypes : uint32_t 
{
    Generic,

    Ping,
    Pong,

    // One to many Msgs
    DistributeNodeInfo,
    DistributeBlock,
    DistributeTransaction,
    
    // One to One Msgs
    Version,
    Verack,

    GetNodeList,
    NodeInfoList,

    GetBlocks,
    Blocks,

    Inv,
    GetData,
    Block,
    
    // Possible "Headers-First" sync method update in the future
    // GetHeaders,
    // Headers,
};

#endif