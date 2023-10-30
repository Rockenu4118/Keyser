#ifndef SERVER_H
#define SERVER_H

#include <Server_Interface.hpp>
#include <rapidjson/document.h>

#include "./MsgTypes.hpp"
#include "../data/version.hpp"
#include "../chain/Chain.hpp"
#include "../chain/Transaction.hpp"
#include "../utils/utils.hpp"


namespace keyser
{
    class Server : public networking::Server_Interface<MsgTypes>
    {
        public:
            Server(uint16_t port) : networking::Server_Interface<MsgTypes>(port)
            {}

        protected: 
            virtual bool onClientConnect(std::shared_ptr<networking::Connection<MsgTypes>> client)
            {
                networking::Message<MsgTypes> msg;
                msg.header.id = MsgTypes::Version;
                msg.push(KEYSER_VERSION);
                client->send(msg);
                return true;
            }

            virtual void onClientDisconnect(std::shared_ptr<networking::Connection<MsgTypes>> client)
            {}

            virtual void onMessage(std::shared_ptr<networking::Connection<MsgTypes>> client, networking::Message<MsgTypes>& msg)
            {
                switch (msg.header.id)
                {
                    case MsgTypes::Ping:
                    {
                        std::cout << "Server Pinged: " << client->getId() << std::endl;
                        msg.push(std::to_string(client->getId()));
                
                        client->send(msg);
                    }
                        break;
                    case MsgTypes::MessageAll:
                    {
                        std::cout << "Messaged all: " << client->getId() << std::endl;
                        networking::Message<MsgTypes> newmsg;
                        newmsg.header.id = MsgTypes::ServerMessage;
                        newmsg.push("Pinged from " + std::to_string(client->getId()));

                        messageAllClients(newmsg, client);
                    }
                        break;
                    case MsgTypes::Transaction:
                    {
                        std::cout << "Transaction incoming." << std::endl;
                        std::string jsonStr;
                        msg.pull(jsonStr);
                        Transaction tx = utils::decodeJson(jsonStr);
                        std::cout << tx << std::endl;
                    }
                        break;
                    default:
                        break;
                }
            }
    
        private:
            Chain* _chain = nullptr;
    };
}

#endif