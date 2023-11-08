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
        class Client : public networking::Client_Interface<MsgTypes>
        {
            public:
                void ping()
                {
                    networking::Message<MsgTypes> msg;
                    msg.header.id = MsgTypes::Ping;
                    send(msg);
                }

                void messageAll()
                {
                    networking::Message<MsgTypes> msg;
                    msg.header.id = MsgTypes::MessageAll;
                    send(msg);
                }

                void sendTransaction(Transaction& transaction)
                {
                    networking::Message<MsgTypes> msg;
                    msg.header.id = MsgTypes::Transaction;
                    std::string msgStr;
                    keyser::utils::encodeJson(msgStr, transaction);
                    msg.push(msgStr);
                    send(msg);
                }

                void sendBlock(Block& block)
                {
                    networking::Message<MsgTypes> msg;
                    msg.header.id = MsgTypes::Block;
                    std::string msgStr;
                    keyser::utils::encodeJson(msgStr, block);
                    msg.push(msgStr);
                    send(msg);
                }

            protected:
                virtual void onMessage(networking::Message<MsgTypes>& msg)
                {
                    switch(msg.header.id)
                    {
                        case MsgTypes::Version:
                            std::cout << msg;
                            msg.print();
                            break;
                        case MsgTypes::Ping:
                            msg.print();
                            break;
                        case MsgTypes::ServerMessage:
                            std::cout << msg;
                            break;
                    }
                }

            private:

        };
    }
}

#endif