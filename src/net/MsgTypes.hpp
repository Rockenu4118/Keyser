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

    Block,

    Inv,
    GetData,
    
    GetHeaders,
    Headers,
};

enum class DataTypes : uint8_t
{
    Error,
    TX,
    Header,
    Block
};

#endif