#include <iostream>
#include <string>
#include <ctime>
#include <nlohmann/json.hpp>

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
    Message msg(MsgTypes::NodeInfoList);

    for (NodeInfo nodeInfo : _activeNodeList)
    {
        nlohmann::json json;
        json["version"] = nodeInfo._version;
        json["alias"]   = nodeInfo._alias;
        json["address"] = nodeInfo._address;
        json["port"]    = nodeInfo._port;
        
        msg.json()["nodeInfoList"].push_back(json);
    }

    msg.serialize();
    message(connection, msg);
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
        case MsgTypes::NodeInfoList:
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

    // Deserialize byte array into json doc
    msg.deserialize();

    // Save external address and add self to list of active nodes
    _selfInfo._address = msg.json()["address"];
    _activeNodeList.insert(_selfInfo);

    // Deserialize incoming node info
    NodeInfo nodeInfo;
    nodeInfo._address = msg.json()["Outbound version"];
    nodeInfo._alias   = msg.json()["Outbound alias"];
    nodeInfo._address = connection->getEndpoint().address().to_string();
    nodeInfo._port    = msg.json()["Outbound port"];

    // Save the port this connection is running their server on
    // Add this node to connectedNodeList
    connection->setHostingPort(nodeInfo._port);
    _connectedNodeList.insert(nodeInfo);

    // Send self info as well as their external ip
    Message newMsg(MsgTypes::Verack);
    newMsg.json()["Outbound version"] = _selfInfo._version;
    newMsg.json()["Outbound alias"]   = _selfInfo._alias;
    newMsg.json()["Outbound port"]    = _selfInfo._port;
    newMsg.json()["address"]          = nodeInfo._address;
    newMsg.serialize();

    message(connection, newMsg);
}

void keyser::Node::handleVerack(std::shared_ptr<Connection> connection, Message& msg)
{
    std::cout << "[NODE] Verack" << std::endl;
    
    // Deserialize byte array into json doc
    msg.deserialize();

    // Deserialize incoming node info
    NodeInfo nodeInfo;
    nodeInfo._version = msg.json()["Outbound version"];
    nodeInfo._alias   = msg.json()["Outbound alias"];
    nodeInfo._address = connection->getEndpoint().address().to_string();
    nodeInfo._port    = msg.json()["Outbound port"];

    // Add info to connected node list and active node list
    _connectedNodeList.insert(nodeInfo);
    _activeNodeList.insert(nodeInfo);
    
    // Save external address
    _selfInfo._address = msg.json()["address"];

    // Add self to list of active nodes
    _activeNodeList.insert(_selfInfo);

    // Get node list if needed
    if (!_recievedNodeList)
    {
        Message msg(MsgTypes::GetNodeList);
        message(_connections.front(), msg);
    }
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
{}

void keyser::Node::handleBlocks(std::shared_ptr<Connection> connection, Message& msg)
{}

void keyser::Node::handleGetNodeList(std::shared_ptr<Connection> connection, Message& msg)
{
    std::cout << "[NODE] Sending Node List" << std::endl;
    nodeInfoStream(connection);
}

void keyser::Node::handleNodeInfo(std::shared_ptr<Connection> connection, Message& msg)
{
    std::cout << "[NODE] Node Info" << std::endl;

    msg.deserialize();

    for (auto& element : msg.json()["nodeInfoList"])
    {
        NodeInfo nodeInfo;
        nodeInfo._version = element["version"];
        nodeInfo._alias   = element["alias"];
        nodeInfo._address = element["address"];
        nodeInfo._port    = element["port"];

        _activeNodeList.insert(nodeInfo);
    }

    _recievedNodeList = true;
}