#ifndef CLIENT_H
#define CLIENT_H

#include <Client_Interface.hpp>
#include <net_message.hpp>

#include "../../data/include/version.hpp"
#include "../../chain/include/Transaction.hpp"
#include "../../chain/include/Chain.hpp"
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

    void sendTransaction()
    {
        networking::Message<MsgTypes> msg;
        msg.header.id = MsgTypes::Transaction;


        send(msg);
    }

    protected:
        virtual void onMessage(networking::Message<MsgTypes>& msg)
        {}

    private:
        keyser::Chain* _chain;
};

#endif