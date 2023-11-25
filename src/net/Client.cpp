#include <string>

#include <Client_Interface.hpp>
#include <net_message.hpp>

#include "../data/version.hpp"
#include "../chain/Transaction.hpp"
#include "../chain/Block.hpp"
#include "../chain/Chain.hpp"
#include "../utils/utils.hpp"
#include "./MsgTypes.hpp"
#include "./Client.hpp"


void keyser::net::Client::ping()
{
    net_core::Message<MsgTypes> msg;
    msg.header.id = MsgTypes::Ping;
    send(msg);
}

void keyser::net::Client::messageAll()
{
    net_core::Message<MsgTypes> msg;
    msg.header.id = MsgTypes::MessageAll;
    send(msg);
}

void keyser::net::Client::sendTransaction(Transaction& transaction)
{
    net_core::Message<MsgTypes> msg;
    msg.header.id = MsgTypes::Transaction;
    std::string msgStr;
    keyser::utils::encodeJson(msgStr, transaction);
    msg.push(msgStr);
    send(msg);
}

void keyser::net::Client::sendBlock(Block& block)
{
    net_core::Message<MsgTypes> msg;
    msg.header.id = MsgTypes::Block;
    std::string msgStr;
    keyser::utils::encodeJson(msgStr, block);
    msg.push(msgStr);
    send(msg);
}

void keyser::net::Client::onMessage(net_core::Message<MsgTypes>& msg)
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