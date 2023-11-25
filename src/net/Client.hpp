#ifndef CLIENT_H
#define CLIENT_H

#include <string>

#include <Client_Interface.hpp>
#include <net_message.hpp>

#include "../data/version.hpp"
#include "../chain/Transaction.hpp"
#include "../chain/Block.hpp"
#include "../chain/Chain.hpp"
#include "../utils/utils.hpp"
#include "./MsgTypes.hpp"


namespace keyser
{
    namespace net
    {
        class Client : public net_core::Client_Interface<MsgTypes>
        {
            public:
                void ping();
                void messageAll();
                void sendTransaction(Transaction& transaction);
                void sendBlock(Block& block);

            protected:
                virtual void onMessage(net_core::Message<MsgTypes>& msg);

            private:

        };
    }
}

#endif