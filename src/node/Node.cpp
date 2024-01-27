#include <iostream>
#include <string>
#include <ctime>

#include "./Node.hpp"
#include "../data/version.hpp"
#include "../data/nodeAddresses.hpp"
#include "../net/MsgTypes.hpp"
#include "../utils/utils.hpp"
#include "../chain/Chain.hpp"
#include "../chain/Block.hpp"
#include "../chain/Transaction.hpp"
#include "../node/NodeInfo.hpp"



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

bool keyser::Node::createTransaction(Transaction& transaction)
{
    double balance = _chain->getAddressBalance(transaction._senderAddress) + _chain->mempool()->getPendingBalance(transaction._senderAddress);

    if (transaction._amount > balance)
        return false;

    _chain->mempool()->addTransaction(transaction);
    distributeTransaction(transaction);
    return true;
}

// void keyser::Node::connectToNetwork()
// {
//     getNodeList();
//
//     sleep(1);
//
//     uint8_t connections = 0;
//
//     for (auto& nodeInfo : _activeNodeList)
//     {
//         if (_selfInfo == nodeInfo)
//         {
//             std::cout << "[NODE] Cannot self connect" << std::endl;
//             continue;
//         }
//
//         if (_connectedNodeList.count(nodeInfo) == 1)
//         {
//             std::cout << "[NODE] No duplicate connections" << std::endl;
//             continue;
//         }
//
//         if (connect(nodeInfo))
//         {
//             _connectedNodeList.insert(nodeInfo);
//             connections++;
//         }
//     }
//
//     std::cout << "Successfully made " << std::to_string(connections) << " connections, distributing self info." << std::endl;
//
//     distributeNodeInfo(_selfInfo);
// }

void keyser::Node::ping()
{
    Message msg(MsgTypes::Ping);
    messageNeighbors(msg);
}

void keyser::Node::distributeNodeInfo(NodeInfo& nodeInfo)
{
    Message msg(MsgTypes::DistributeNodeInfo);
    msg.insert(nodeInfo);
    messageNeighbors(msg);
}

void keyser::Node::distributeTransaction(Transaction& transaction)
{
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

void keyser::Node::getBlocks()
{
    Message msg(MsgTypes::GetBlocks);
    message(_connections.front(), msg);
}

void keyser::Node::sendBlocks(std::shared_ptr<Connection> connection)
{

}

void keyser::Node::nodeInfoStream(std::shared_ptr<Connection> connection)
{
    for (NodeInfo nodeInfo : _activeNodeList)
    {
        Message msg(MsgTypes::NodeInfo);
        msg.insert(nodeInfo);
        message(connection, msg);
    }
}

keyser::Chain* keyser::Node::chain()
{
    return _chain;
}

bool keyser::Node::allowConnect(std::shared_ptr<Connection> connection)
{ return true; }

void keyser::Node::onIncomingConnect(std::shared_ptr<Connection> connection) 
{}

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
        case MsgTypes::Version:
            handleVersion(connection, msg);
            break;
        case MsgTypes::Verack:
            handleVerack(connection, msg);
            break;
        case MsgTypes::GetBlocks:
            handleGetBlocks(connection, msg);
            break;
        case MsgTypes::Block:
            handleBlocks(connection, msg);
            break;
        case MsgTypes::GetNodeList:
            handleGetNodeList(connection, msg);
            break;
        case MsgTypes::NodeInfo:
            handleNodeInfo(connection, msg);
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

void keyser::Node::handleVersion(std::shared_ptr<Connection> connection, Message& msg)
{
    std::cout << "[NODE] Version" << std::endl;
    // Save external address
    msg.deserialize();
    msg.get("address", _selfInfo._address);

    // Deserialize incoming node info
    NodeInfo nodeInfo;
    msg.get("Outbound version", nodeInfo._version);
    msg.get("Outbound alias", nodeInfo._alias);
    nodeInfo._address = connection->getEndpoint().address().to_string();
    msg.get("Outbound port", nodeInfo._port);
    _connectedNodeList.insert(nodeInfo);

    // Send self info as well as their external ip
    Message newMsg(MsgTypes::Verack);
    newMsg.edit("Outbound version", _selfInfo._version);
    newMsg.edit("Outbound alias", _selfInfo._alias);
    newMsg.edit("Outbound port", _selfInfo._port);
    newMsg.edit("address", nodeInfo._address);
    newMsg.serialize();

    message(connection, newMsg);
}

void keyser::Node::handleVerack(std::shared_ptr<Connection> connection, Message& msg)
{
    std::cout << "[NODE] Verack" << std::endl;
    // Deserialize incoming info
    msg.deserialize();
    NodeInfo nodeInfo;
    msg.get("Outbound version", nodeInfo._version);
    msg.get("Outbound alias", nodeInfo._alias);
    nodeInfo._address = connection->getEndpoint().address().to_string();
    msg.get("Outbound port", nodeInfo._port);
    
    // Save external address
    msg.get("address", _selfInfo._address);

    // Add self to list of active nodes
    _activeNodeList.insert(_selfInfo);
}

void keyser::Node::handleDistributeNodeInfo(std::shared_ptr<Connection> connection, Message& msg)
{
    std::cout << "[NODE] Distribute Node Info" << std::endl;
    NodeInfo nodeInfo;
    msg.extract(nodeInfo);

    if (_activeNodeList.count(nodeInfo) == 1)
        return;

    _activeNodeList.insert(nodeInfo);
    messageNeighbors(msg, connection);
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

void keyser::Node::handleGetBlocks(std::shared_ptr<Connection> connection, Message& msg)
{
    // TODO - make better and test more
    std::cout << "Recieved chain req" << std::endl;

    // blockStream();

    // for (int i = 1 ; i < _chain->blocks().size() ; i++)
    // {
    //     Message msg;
    //     msg.header.id = MsgTypes::DistributeBlock;
    //     std::string msgStr;
    //     keyser::utils::encodeJson(msgStr, *_chain->blocks().at(i));
    //     // msg.push(msgStr);
    //     message(connection, msg);
    // }
}

void keyser::Node::handleBlocks(std::shared_ptr<Connection> connection, Message& msg)
{}

void keyser::Node::handleGetNodeList(std::shared_ptr<Connection> connection, Message& msg)
{
    std::cout << "[NODE] Get Node List" << std::endl;
    nodeInfoStream(connection);
}

void keyser::Node::handleNodeInfo(std::shared_ptr<Connection> connection, Message& msg)
{
    std::cout << "[NODE] Node Info" << std::endl;
    NodeInfo nodeInfo;
    msg.extract(nodeInfo);
    _activeNodeList.insert(nodeInfo);
}