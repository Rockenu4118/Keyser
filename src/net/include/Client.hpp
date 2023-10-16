#ifndef CLIENT_H
#define CLIENT_H

#include <Client_Interface.hpp>

#include "../../data/include/version.hpp"
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
};

#endif