#ifndef MSG_TYPES_H
#define MSG_TYPES_H

enum class MsgTypes : uint32_t 
{
    Generic,

    Ping,

    // One to many Msgs
    DistributeNodeInfo,
    DistributeBlock,
    DistributeTransaction,
    
    // One to One Msgs
    NodeInfoReq,
    NodeInfoRes,

    ChainReq,
    ChainRes,

    ActiveNodeListReq,
    ActiveNodeListRes,

    // Used for InitBlockDownload or Retrieving active node list
    BlockStream,
    NodeInfoStream,
};

#endif