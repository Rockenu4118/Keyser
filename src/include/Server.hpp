#ifndef SERVER_H
#define SERVER_H

#include <Server_Interface.hpp>
#include "./MsgTypes.hpp"

// enum class CustomMsgTypes : uint32_t
// {
//     ServerAccept,
//     ServerDeny,
//     ServerPing
// };

class Server : public networking::Server_Interface<MsgTypes>
{
    public:
        Server(uint16_t port) : networking::Server_Interface<MsgTypes>(port)
        {

        }

    protected: 
        virtual bool onClientConnect(std::shared_ptr<networking::Connection<MsgTypes>> client)
        {
            return true;
        }

        virtual void onClientDisconnect(std::shared_ptr<networking::Connection<MsgTypes>> client)
        {

        }

        virtual void onMessage(std::shared_ptr<networking::Connection<MsgTypes>> client, networking::Message<MsgTypes>& msg)
        {
            switch (msg.header.id)
            {
            case MsgTypes::Ping:
            {
                std::cout << "Server Pinged: " << client->getId() << std::endl;
                client->send(msg);
            }
                break;
            case MsgTypes::MessageAll:
            {
                std::cout << "Messaged all: " << client->getId() << std::endl;
                networking::Message<MsgTypes> newmsg;
                newmsg.header.id = MsgTypes::ServerMessage;
                newmsg << 200;
                messageAllClients(newmsg, client);
            }
                break;
            default:
                break;
            }
        }
};

#endif