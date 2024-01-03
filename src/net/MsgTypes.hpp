#ifndef MSG_TYPES_H
#define MSG_TYPES_H

enum class MsgTypes : uint32_t 
{
    Ping,

    DistributeAddr,
    DistributeBlock,
    DistributeTransaction,

    RelayAddr,
    RelayBlock,
    RelayTransaction,

    VersionReq,
    VersionRes,
    
    InitBlockDownloadReq,
    InitBlockDownloadRes,

    AddrListReq,
    AddrListRes,
};

#endif