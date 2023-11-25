#ifndef SERVER_H
#define SERVER_H

#include <Server_Interface.hpp>

#include "./MsgTypes.hpp"
#include "../data/version.hpp"
#include "../chain/Chain.hpp"
#include "../chain/Transaction.hpp"
#include "../chain/Block.hpp"
#include "../utils/utils.hpp"


namespace keyser
{
    namespace net
    {
        class Server : public net_core::Server_Interface<MsgTypes>
        {
            public:
                Server(uint16_t port, Chain* chain);

            protected: 
                virtual bool onClientConnect(std::shared_ptr<net_core::Connection<MsgTypes>> client)
                {
                    net_core::Message<MsgTypes> msg;
                    msg.header.id = MsgTypes::Version;
                    msg.push(KEYSER_VERSION);
                    client->send(msg);
                    return true;
                }

                virtual void onClientDisconnect(std::shared_ptr<net_core::Connection<MsgTypes>> client)
                {}

                virtual void onMessage(std::shared_ptr<net_core::Connection<MsgTypes>> client, net_core::Message<MsgTypes>& msg)
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
                            net_core::Message<MsgTypes> newmsg;
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
                            Transaction transaction;
                            utils::decodeJson(transaction, jsonStr);
                            std::cout << (transaction.isValid() ? "Valid" : "Not valid") << std::endl;
                        }
                            break;
                        case MsgTypes::Block:
                        {
                            std::cout << "Block incoming." << std::endl;
                            std::string jsonStr;
                            msg.pull(jsonStr);
                            Block block;
                            utils::decodeJson(block, jsonStr);
                            _chain->addBlock(block);
                        }
                        default:
                            break;
                    }
                }
    
            private:
                Chain* _chain = nullptr;
        };
    }
}

#endif