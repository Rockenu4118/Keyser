#include <iostream>
#include <string>
#include <ctime>
#include <nlohmann/json.hpp>

#include "./Node.hpp"
#include "../data/version.hpp"
#include "../data/nodeAddresses.hpp"
#include "../data/keys.hpp"
#include "../net/MsgTypes.hpp"
#include "../utils/utils.hpp"
#include "../chain/Chain.hpp"
#include "../chain/Block.hpp"
#include "../chain/Transaction.hpp"
#include "../node/NodeInfo.hpp"
#include "../wallet/WalletManager.hpp"
#include "../wallet/Wallet.hpp"
#include "../storage/StorageEngine.hpp"


keyser::Node::Node(uint16_t port) : NetInterface(port)
{
    // Test wallets
    keyser::Wallet ajWallet("AJ", key1);
    _walletManager.addWallet(ajWallet);

    keyser::Wallet guyWallet("Guy", key2);
    _walletManager.addWallet(guyWallet);
}

void keyser::Node::run()
{
    _storageEngine.loadChain(_blocks);
    _storageEngine.loadWallets(_walletManager);

    _chainAssemblerThr = std::thread([this]() { buildChain(); });

    startServer();
}

void keyser::Node::buildChain()
{
    while (!_completedPeerDiscovery)
        sleep(1);

    std::cout << "completed peer discovery" << std::endl;

    std::shared_ptr<Connection> longestChain;

    for (auto connection : _connections)
    {
        if (!longestChain)
        {
            longestChain = connection;
            continue;
        }

        if (longestChain->getChainHeight() < connection->getChainHeight())
            longestChain = connection;
    }

    std::cout << longestChain->getId() << ": has longest chain" << std::endl;

    Message msg(MsgTypes::GetBlocks);
    message(longestChain, msg);
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
        _miningThr = std::thread([this]() { while (1) { mineBlock("aj"); }; });
    else
        _miningThr = std::thread([this]() { mineBlock("aj"); });

    _miningThr.detach();
}

void keyser::Node::mineBlock(std::string rewardAddress)
{
    Block newBlock(getCurrBlock()->_index + 1, time(NULL), getCurrBlock()->_hash, 100, rewardAddress, _mempool.popLeadingTransactions());

    newBlock.calcValidHash(calcDifficulty());
    std::cout << "[CHAIN] Block Mined." << std::endl;

    addBlock(std::move(newBlock));

    distributeBlock(*getCurrBlock());

    _miningStatus = false;                     
}

bool keyser::Node::createTransaction(Transaction& transaction)
{
    double balance = getAddressBalance(transaction._senderAddress) + _mempool.getPendingBalance(transaction._senderAddress);

    if (transaction._amount > balance)
        return false;

    _mempool.addTransaction(transaction);
    distributeTransaction(transaction);
    return true;
}

void keyser::Node::ping()
{
    Message msg(MsgTypes::Ping);
    messageNeighbors(msg);
}

void keyser::Node::pong() {}

void keyser::Node::version(std::shared_ptr<Connection> connection)
{
    // Send self info as well as their external address
    Message msg(MsgTypes::Version);
    msg.json()["Outbound version"] = _selfInfo._version;
    msg.json()["Outbound alias"]   = _selfInfo._alias;
    msg.json()["Outbound port"]    = _selfInfo._port;
    msg.json()["chainHeight"]      = getHeight();
    msg.json()["address"]          = connection->getEndpoint().address().to_string();
    msg.serialize();
    message(connection, msg);
}

void keyser::Node::verack(std::shared_ptr<Connection> connection){}

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
    Message newMsg(MsgTypes::Blocks);

    for (int i = 1 ; i < blocks().size() ; i++)
    {
        nlohmann::json json;
        utils::encodeJson(json, *blocks().at(i));
        newMsg.json()["blocks"].push_back(json);
    }

    newMsg.serialize();
    message(connection, newMsg);
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

keyser::Mempool& keyser::Node::mempool()
{
    return _mempool;
}

keyser::WalletManager& keyser::Node::walletManager()
{
    return _walletManager;
}

bool keyser::Node::allowConnect(std::shared_ptr<Connection> connection)
{ return true; }

void keyser::Node::onOutgoingConnect(std::shared_ptr<Connection> connection)
{
    version(connection);
}

void keyser::Node::onIncomingConnect(std::shared_ptr<Connection> connection) 
{}

void keyser::Node::onDisconnect(std::shared_ptr<Connection> connection)
{}

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
        case MsgTypes::Blocks:
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
    std::cout << utils::localTimestamp() << "Version" << std::endl;

    // Deserialize byte array into json doc
    msg.deserialize();

    // Save external address and add self to list of active nodes
    _selfInfo._address = msg.json()["address"];
    _activeNodeList.insert(_selfInfo);

    // Deserialize incoming node info
    NodeInfo nodeInfo;
    nodeInfo._version = msg.json()["Outbound version"];
    nodeInfo._alias   = msg.json()["Outbound alias"];
    nodeInfo._address = connection->getEndpoint().address().to_string();
    nodeInfo._port    = msg.json()["Outbound port"];

    connection->setChainHeight(msg.json()["chainHeight"]);

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
    std::cout << utils::localTimestamp() << "Verack" << std::endl;
    
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
    std::cout << utils::localTimestamp() << "Distribute Node Info" << std::endl;
    NodeInfo nodeInfo;
    msg.extract(nodeInfo);

    if (_activeNodeList.count(nodeInfo))
        return;

    _activeNodeList.insert(nodeInfo);
    addPotentialConnection(nodeInfo);
    messageNeighbors(msg, connection);
}

void keyser::Node::handleDistributeTransaction(std::shared_ptr<Connection> connection, Message& msg)
{
    std::cout << utils::localTimestamp() << "Transaction" << std::endl;

    Transaction transaction;
    msg.extract(transaction);

    if (_mempool.addTransaction(transaction))
        messageNeighbors(msg, connection);
}

void keyser::Node::handleDistributeBlock(std::shared_ptr<Connection> connection, Message& msg)
{
    std::cout << utils::localTimestamp() << "Block" << std::endl;

    Block block;
    msg.extract(block);

    if (addBlock(block))
        messageNeighbors(msg, connection);
}

void keyser::Node::handleGetBlocks(std::shared_ptr<Connection> connection, Message& msg)
{   
    std::cout << utils::localTimestamp() << "Get Blocks" << std::endl;
    sendBlocks(connection); 
}

void keyser::Node::handleBlocks(std::shared_ptr<Connection> connection, Message& msg)
{
    std::cout << utils::localTimestamp() << "Blocks" << std::endl;
    msg.deserialize();

    for (auto& element : msg.json()["blocks"])
    {
        Block block;
        utils::decodeJson(block, element);
        addBlock(block);
    }

    _recievedChain = true;
}

void keyser::Node::handleGetNodeList(std::shared_ptr<Connection> connection, Message& msg)
{
    std::cout << utils::localTimestamp() << "Sending Node List" << std::endl;
    nodeInfoStream(connection);
}

void keyser::Node::handleNodeInfo(std::shared_ptr<Connection> connection, Message& msg)
{
    std::cout << utils::localTimestamp() << "Info" << std::endl;

    msg.deserialize();

    for (auto& element : msg.json()["nodeInfoList"])
    {
        NodeInfo nodeInfo;
        nodeInfo._version = element["version"];
        nodeInfo._alias   = element["alias"];
        nodeInfo._address = element["address"];
        nodeInfo._port    = element["port"];

        _activeNodeList.insert(nodeInfo);
        addPotentialConnection(nodeInfo);
    }

    _recievedNodeList = true;
}