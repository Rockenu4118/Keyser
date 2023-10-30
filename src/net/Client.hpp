#ifndef CLIENT_H
#define CLIENT_H

#include <string>

#include <Client_Interface.hpp>
#include <net_message.hpp>

#include "../data/version.hpp"
#include "../chain/Transaction.hpp"
#include "../chain/Chain.hpp"
#include "../utils/utils.hpp"
#include "./MsgTypes.hpp"


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

    void sendTransaction(keyser::Transaction& transaction)
    {
        networking::Message<MsgTypes> msg;
        msg.header.id = MsgTypes::Transaction;
        msg.push(keyser::utils::encodeJson(transaction));
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
        keyser::Chain* _chain = nullptr;
};

#endif