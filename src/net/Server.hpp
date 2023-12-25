#ifndef SERVER_H
#define SERVER_H

// #include <Server_Interface.hpp>

#include "./MsgTypes.hpp"
#include "../data/version.hpp"
#include "../chain/Chain.hpp"
#include "../chain/Transaction.hpp"
#include "../chain/Block.hpp"
#include "../utils/utils.hpp"


// namespace keyser
// {
//     namespace net
//     {
//         class Server : public net_core::Server_Interface<MsgTypes>
//         {
//             public:
//                 Server(uint16_t port, Chain* chain);

//             protected: 
//                 virtual bool onClientConnect(std::shared_ptr<net_core::Connection<MsgTypes>> client);
//                 virtual void onClientDisconnect(std::shared_ptr<net_core::Connection<MsgTypes>> client);
//                 virtual void onMessage(std::shared_ptr<net_core::Connection<MsgTypes>> client, net_core::Message<MsgTypes>& msg);
    
//             private:
//                 void handlePing(std::shared_ptr<net_core::Connection<MsgTypes>> client, net_core::Message<MsgTypes>& msg);
//                 void handleTransaction(std::shared_ptr<net_core::Connection<MsgTypes>> client, net_core::Message<MsgTypes>& msg);
//                 void handleBlock(std::shared_ptr<net_core::Connection<MsgTypes>> client, net_core::Message<MsgTypes>& msg);

//                 Chain* _chain = nullptr;
//         };
//     }
// }

#endif