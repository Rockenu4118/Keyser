#include <Server_Interface.hpp>

#include "./MsgTypes.hpp"
#include "../data/version.hpp"
#include "../chain/Chain.hpp"
#include "../chain/Transaction.hpp"
#include "../chain/Block.hpp"
#include "../utils/utils.hpp"
#include "./Server.hpp"


keyser::net::Server::Server(uint16_t port, Chain* chain) : net_core::Server_Interface<MsgTypes>(port)
{
    _chain = chain;
}

bool keyser::net::Server::onClientConnect(std::shared_ptr<net_core::Connection<MsgTypes>> client)
{
    net_core::Message<MsgTypes> msg;
    msg.header.id = MsgTypes::Version;
    msg.push(KEYSER_VERSION);
    client->send(msg);
    return true;
}

void keyser::net::Server::onClientDisconnect(std::shared_ptr<net_core::Connection<MsgTypes>> client)
{}

void keyser::net::Server::onMessage(std::shared_ptr<net_core::Connection<MsgTypes>> client, net_core::Message<MsgTypes>& msg)
{
    switch (msg.header.id)
    {
        case MsgTypes::Ping:
            handlePing(client, msg);
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
            handleTransaction(client, msg);
            break;
        case MsgTypes::Block:
            handleBlock(client, msg);
            break;
        default:
            break;
    }
}

void keyser::net::Server::handlePing(std::shared_ptr<net_core::Connection<MsgTypes>> client, net_core::Message<MsgTypes>& msg)
{
    std::cout << "[Server] Ping: " << client->getId() << std::endl;
    msg.push(std::to_string(client->getId()));
                
    client->send(msg);
}

void keyser::net::Server::handleTransaction(std::shared_ptr<net_core::Connection<MsgTypes>> client, net_core::Message<MsgTypes>& msg)
{
    std::cout << "[Server] Transaction" << std::endl;
    std::string jsonStr;
    Transaction transaction;
    msg.pull(jsonStr);
    utils::decodeJson(transaction, jsonStr);
    _chain->mempool()->addTransaction(transaction);
}

void keyser::net::Server::handleBlock(std::shared_ptr<net_core::Connection<MsgTypes>> client, net_core::Message<MsgTypes>& msg)
{
    std::cout << "[Server] Block" << std::endl;
    std::string jsonStr;
    msg.pull(jsonStr);
    Block block;
    utils::decodeJson(block, jsonStr);
    _chain->addBlock(block);
}
