#ifndef NODE_H
#define NODE_H

#include <set>

#include <Net_Interface.hpp>
#include <Net_Connection.hpp>

#include "../net/MsgTypes.hpp"
#include "../chain/Chain.hpp"
#include "../chain/Transaction.hpp"
#include "../chain/Block.hpp"


namespace keyser
{
    class Node : public net_core::Net_Interface<MsgTypes>
    {
        public:
            Node(uint16_t port);

            void beginMining(bool continuous);
            void mineSingleBlock();
            void mineContinuously();

            void InitBlockDownload();

            void sendVersion();
            void sendActiveNodes();
            void sendTransaction(Transaction& transaction);
            void sendBlock(Block& block);
            void ping();

            Chain* chain();

        protected:
            virtual bool onConnect(std::shared_ptr<net_core::Net_Connection<MsgTypes>> connection);
            virtual void onDisconnect(std::shared_ptr<net_core::Net_Connection<MsgTypes>> connection);
            virtual void onMessage(std::shared_ptr<net_core::Net_Connection<MsgTypes>> connection, net_core::Message<MsgTypes>& msg);

        private:
            // Msg Handlers
            void handlePing(std::shared_ptr<net_core::Net_Connection<MsgTypes>> connection, net_core::Message<MsgTypes>& msg);
            void handleTransaction(std::shared_ptr<net_core::Net_Connection<MsgTypes>> connection, net_core::Message<MsgTypes>& msg);
            void handleBlock(std::shared_ptr<net_core::Net_Connection<MsgTypes>> connection, net_core::Message<MsgTypes>& msg);
            void handleInitBlockDownload(std::shared_ptr<net_core::Net_Connection<MsgTypes>> connection, net_core::Message<MsgTypes>& msg);

            // Node members
            Chain* _chain = nullptr;

            

            
            std::thread _miningThr;

            bool _miningStatus;
        };
}

#endif