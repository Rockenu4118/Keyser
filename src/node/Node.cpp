#include <iostream>
#include <string>

#include "./Node.hpp"
#include "../data/version.hpp"
#include "../net/MsgTypes.hpp"
#include "../utils/utils.hpp"
#include "../chain/Chain.hpp"
#include "../chain/Block.hpp"
#include "../chain/Transaction.hpp"



keyser::Node::Node(uint16_t port) : net_core::Net_Interface<MsgTypes>(port)
{
    _chain  = new keyser::Chain(4, 100);
}

void keyser::Node::beginMining(bool continuous)
{
    if (_miningStatus == true)
    {
        std::cout << "[CHAIN] Mining In Progress" << std::endl;
        return;
    }

    _miningStatus = true;

    if (continuous)
        _miningThr = std::thread([this]() { mineContinuously(); });
    else
        _miningThr = std::thread([this]() { mineSingleBlock(); });

    _miningThr.detach();
}

void keyser::Node::mineContinuously()
{
    while (1)
    {
        _chain->mineBlock("aj");
        std::cout << "[CHAIN] Block Mined." << std::endl;
        net_core::Message<MsgTypes> msg;
        msg.header.id = MsgTypes::Block;
        std::string msgStr;
        keyser::utils::encodeJson(msgStr, *_chain->getCurrBlock());
        msg.push(msgStr);
        messageNeighbors(msg);
    }
}

void keyser::Node::mineSingleBlock()
{
    _chain->mineBlock("aj");
    std::cout << "[CHAIN] Block Mined." << std::endl;
    net_core::Message<MsgTypes> msg;
    msg.header.id = MsgTypes::Block;
    std::string msgStr;
    keyser::utils::encodeJson(msgStr, *_chain->getCurrBlock());
    msg.push(msgStr);
    messageNeighbors(msg);
    _miningStatus = false;
}

void keyser::Node::InitBlockDownload()
{
    net_core::Message<MsgTypes> msg;
    msg.header.id = MsgTypes::InitBlockDownload;
    message(_connections.front(), msg);
}

void keyser::Node::sendActiveNodes()
{
    // for (auto& nodeInfo : _activeNodes)
    // {

    // }
}

void keyser::Node::sendTransaction(Transaction& transaction)
{
    _chain->mempool()->addTransaction(transaction);
    net_core::Message<MsgTypes> msg;
    msg.header.id = MsgTypes::Transaction;
    std::string msgStr;
    keyser::utils::encodeJson(msgStr, transaction);
    msg.push(msgStr);
    messageNeighbors(msg);
}

void keyser::Node::sendBlock(Block& block)
{
    net_core::Message<MsgTypes> msg;
    msg.header.id = MsgTypes::Block;
    std::string msgStr;
    keyser::utils::encodeJson(msgStr, block);
    msg.push(msgStr);
    messageNeighbors(msg);
}

void keyser::Node::ping()
{
    net_core::Message<MsgTypes> msg;
    msg.header.id = MsgTypes::Ping;
    messageNeighbors(msg);
}

keyser::Chain* keyser::Node::chain()
{
    return _chain;
}

bool keyser::Node::onConnect(std::shared_ptr<net_core::Net_Connection<MsgTypes>> connection) 
{ 
    // TODO - version msg upon connection
    // TODO - add connection to active node list
    std::cout << "Sending active nodes list" << std::endl;
    // _activeNodes
    return true; 
}

void keyser::Node::onDisconnect(std::shared_ptr<net_core::Net_Connection<MsgTypes>> connection)
{}

void keyser::Node::onMessage(std::shared_ptr<net_core::Net_Connection<MsgTypes>> connection, net_core::Message<MsgTypes>& msg)
{
    switch (msg.header.id)
    {
        case MsgTypes::Ping:
            handlePing(connection, msg);
            break;
        case MsgTypes::Transaction:
            handleTransaction(connection, msg);
            break;
        case MsgTypes::Block:
            handleBlock(connection, msg);
            break;
        case MsgTypes::InitBlockDownload:
            handleInitBlockDownload(connection, msg);
            break;
        default:
            std::cout << "[NODE] Unknown Msg Type" << std::endl;
            break;
    }
}

void keyser::Node::handlePing(std::shared_ptr<net_core::Net_Connection<MsgTypes>> connection, net_core::Message<MsgTypes>& msg)
{
    std::cout << "[NODE] Ping: " << connection->getId() << std::endl;
}

void keyser::Node::handleTransaction(std::shared_ptr<net_core::Net_Connection<MsgTypes>> connection, net_core::Message<MsgTypes>& msg)
{
    std::cout << "[NODE] Transaction" << std::endl;
    messageNeighbors(msg, connection);
    std::string jsonStr;
    Transaction transaction;
    msg.pull(jsonStr);
    utils::decodeJson(transaction, jsonStr);
    _chain->mempool()->addTransaction(transaction);
}

void keyser::Node::handleBlock(std::shared_ptr<net_core::Net_Connection<MsgTypes>> connection, net_core::Message<MsgTypes>& msg)
{
    std::cout << "[NODE] Block" << std::endl;
    messageNeighbors(msg, connection);
    std::string jsonStr;
    msg.pull(jsonStr);
    Block block;
    utils::decodeJson(block, jsonStr);
    _chain->addBlock(block);
}

void keyser::Node::handleInitBlockDownload(std::shared_ptr<net_core::Net_Connection<MsgTypes>> connection, net_core::Message<MsgTypes>& msg)
{
    // TODO - make better and test more

    std::cout << "Recieved init download req" << std::endl;

    for (int i = 1 ; i < _chain->blocks().size() ; i++)
    {
        net_core::Message<MsgTypes> msg;
        msg.header.id = MsgTypes::Block;
        std::string msgStr;
        keyser::utils::encodeJson(msgStr, *_chain->blocks().at(i));
        msg.push(msgStr);
        message(connection, msg);
    }
}