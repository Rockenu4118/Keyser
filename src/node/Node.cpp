#include <iostream>
#include <string>
#include <ctime>

#include "./Node.hpp"
#include "../data/version.hpp"
#include "../net/MsgTypes.hpp"
#include "../utils/utils.hpp"
#include "../chain/Chain.hpp"
#include "../chain/Block.hpp"
#include "../chain/Transaction.hpp"
#include "../node/NodeInfo.hpp"
#include "../data/version.hpp"



keyser::Node::Node(uint16_t port) : Node_Interface(port)
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
        distributeBlock(*_chain->getCurrBlock());
    }
}

void keyser::Node::mineSingleBlock()
{
    _chain->mineBlock("aj");
    std::cout << "[CHAIN] Block Mined." << std::endl;
    distributeBlock(*_chain->getCurrBlock());
    _miningStatus = false;
}

void keyser::Node::sendActiveNodeList()
{
    for (auto nodeInfo : _activeNodeList)
    {
        distributeNodeInfo(nodeInfo);
    }
}

void keyser::Node::InitBlockDownload()
{
    Message msg(MsgTypes::ChainReq);
    message(_connections.front(), msg);
}

void keyser::Node::distributeNodeInfo(NodeInfo& nodeInfo)
{
    Message msg(MsgTypes::DistributeNodeInfo);
    msg.insert(nodeInfo);
    messageNeighbors(msg);
}

void keyser::Node::distributeTransaction(Transaction& transaction)
{
    _chain->mempool()->addTransaction(transaction);
    Message msg(MsgTypes::DistributeTransaction);
    msg.insert(transaction);
    messageNeighbors(msg);
}

void keyser::Node::distributeBlock(Block& block)
{
    Message msg(MsgTypes::DistributeBlock);
    msg.insert(block);
    messageNeighbors(msg);
}

void keyser::Node::ping()
{
    Message msg(MsgTypes::Ping);
    messageNeighbors(msg);
}

keyser::Chain* keyser::Node::chain()
{
    return _chain;
}

void keyser::Node::onOutgoingConnect(std::shared_ptr<Connection> connection)
{}

bool keyser::Node::allowConnect(std::shared_ptr<Connection> connection) { return true; }

void keyser::Node::onIncomingConnect(std::shared_ptr<Connection> connection) 
{
    // TODO - add connection to active node list
    Message msg(MsgTypes::NodeInfoReq);
    NodeInfo nodeInfo;
    nodeInfo._version = KEYSER_VERSION;
    nodeInfo._alias = "myMFNode";
    nodeInfo._port = _port;
    msg.insert(nodeInfo);
    message(connection, msg);
}

void keyser::Node::onDisconnect(std::shared_ptr<Connection> connection)
{
    // TODO - remove address from active nodes list
}

void keyser::Node::onMessage(std::shared_ptr<Connection> connection, Message& msg)
{
    switch (msg.header.id)
    {
        case MsgTypes::Generic:
            std::cout << "[NODE] Generic msg" << std::endl;
            break;
        case MsgTypes::Ping:
            handlePing(connection, msg);
            break;
        case MsgTypes::DistributeNodeInfo:
            handleDistributeNodeInfo(connection, msg);
            break;
        case MsgTypes::DistributeBlock:
            handleDistributeBlock(connection, msg);
            break;
        case MsgTypes::DistributeTransaction:
            handleDistributeTransaction(connection, msg);
            break;
        case MsgTypes::NodeInfoReq:
            handleNodeInfoReq(connection, msg);
            break;
        case MsgTypes::NodeInfoRes:
            handleNodeInfoRes(connection, msg);
            break;
        case MsgTypes::ChainReq:
            handleInitBlockDownload(connection, msg);
            break;
        default:
            std::cout << "[NODE] Unknown Msg Type" << std::endl;
            break;
    }
}

void keyser::Node::handlePing(std::shared_ptr<Connection> connection, Message& msg)
{
    std::cout << utils::localTimestamp() << "Ping: " << connection->getId() << std::endl;
}

void keyser::Node::handleNodeInfoReq(std::shared_ptr<Connection> connection, Message& msg)
{
    std::cout << "[NODE] Node Info Req" << std::endl;
    NodeInfo incomingNodeInfo;
    msg.extract(incomingNodeInfo);
    incomingNodeInfo._address = connection->getEndpoint().address().to_string();
    _activeNodeList.insert(incomingNodeInfo);

    Message newMsg(MsgTypes::NodeInfoRes);
    NodeInfo outgoingNodeInfo;
    outgoingNodeInfo._version = KEYSER_VERSION;
    outgoingNodeInfo._alias = "node";
    outgoingNodeInfo._port = _port;
    newMsg.insert(outgoingNodeInfo);
    message(connection, newMsg);
}

void keyser::Node::handleNodeInfoRes(std::shared_ptr<Connection> connection, Message& msg)
{
    std::cout << "[NODE] Node Info Res" << std::endl;
    NodeInfo nodeInfo;
    msg.extract(nodeInfo);
    nodeInfo._address = connection->getEndpoint().address().to_string();
    _activeNodeList.insert(nodeInfo);
    distributeNodeInfo(nodeInfo);
}

void keyser::Node::handleDistributeNodeInfo(std::shared_ptr<Connection> connection, Message& msg)
{
    std::cout << "[NODE] Node Info" << std::endl;
    messageNeighbors(msg, connection);
    NodeInfo nodeInfo;
    msg.extract(nodeInfo);
    _activeNodeList.insert(nodeInfo);
}

void keyser::Node::handleDistributeTransaction(std::shared_ptr<Connection> connection, Message& msg)
{
    std::cout << "[NODE] Transaction" << std::endl;
    messageNeighbors(msg, connection);
    Transaction transaction;
    msg.extract(transaction);
    _chain->mempool()->addTransaction(transaction);
}

void keyser::Node::handleDistributeBlock(std::shared_ptr<Connection> connection, Message& msg)
{
    std::cout << "[NODE] Block" << std::endl;
    messageNeighbors(msg, connection);
    Block block;
    msg.extract(block);
    _chain->addBlock(block);
}

void keyser::Node::handleInitBlockDownload(std::shared_ptr<Connection> connection, Message& msg)
{
    // TODO - make better and test more

    std::cout << "Recieved init download req" << std::endl;

    // for (int i = 1 ; i < _chain->blocks().size() ; i++)
    // {
    //     Message msg;
    //     // msg.header.id = MsgTypes::DistributeBlock;
    //     std::string msgStr;
    //     keyser::utils::encodeJson(msgStr, *_chain->blocks().at(i));
    //     // msg.push(msgStr);
    //     message(connection, msg);
    // }
}