#include <iostream>
#include <string>
#include <ctime>
#include <nlohmann/json.hpp>

#include "./Node.hpp"
#include "../net/NetInterface.hpp"
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

    _validationEngine = new ValidationEngine(*this);
}

void keyser::Node::run()
{
    _storageEngine.loadChain();
    _storageEngine.loadWallets();

    boost::asio::io_context::work idleWork(_context);
    _contextThread = std::thread([this]() { _context.run(); });  

    // Run thread to handle messsages
    _responseThread = std::thread([this]() { update(); }); 

    // Begin peer discovery / connection management
    _connectionManagementThread = std::thread([this]() { managePeerConnections(); });

    // Activate thread to dispose of invalid connections
    _connectionRemovalThread = std::thread([this]() { removeInvalidConnections(); });

    _ibdThr = std::thread([this]() { initialBlockDownload(); });

    startServer();
}

keyser::Node::Status keyser::Node::getStatus()
{
    return _status;
}

void keyser::Node::initialBlockDownload()
{
    while (connectionCount() == 0)
        sleep(1);

    _status = Status::InitialBlockDownload;

    getBlocks();
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

    _validationEngine->validateBlock(std::move(newBlock));

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

void keyser::Node::completedInitialBlockDownload()
{
    _status = Status::Online;

    // Can now advertise self as a node on the network
    Message msg(MsgTypes::DistributeNodeInfo);
    msg.insert(_selfInfo);
    messageNeighbors(msg);  
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

void keyser::Node::verack(std::shared_ptr<Connection> connection)
{
    Message newMsg(MsgTypes::Verack);
    newMsg.json()["Outbound version"] = _selfInfo._version;
    newMsg.json()["Outbound alias"]   = _selfInfo._alias;
    newMsg.json()["Outbound port"]    = _selfInfo._port;
    newMsg.json()["address"]          = connection->getEndpoint().address().to_string();
    newMsg.serialize();

    message(connection, newMsg);
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
    msg.json()["topBlock"] = getCurrBlock()->_hash;
    msg.serialize();

    message(syncNode(), msg);
}

void keyser::Node::sendBlocks(std::shared_ptr<Connection> connection, int blockIndex)
{
    Message msg(MsgTypes::Block);

    msg.insert(*_blocks.at(blockIndex));

    message(connection, msg);
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

void keyser::Node::getData()
{
    Message msg(MsgTypes::GetData);

    for (auto element : _inventory)
        msg.json()["blockIndexes"].push_back(element);

    msg.serialize();

    message(syncNode(), msg);
}

void keyser::Node::inv(std::shared_ptr<Connection> connection, int startingBlock)
{
    Message msg(MsgTypes::Inv);

    if (startingBlock == getHeight() - 1)
    {
        message(connection, msg);
        return;
    }
    
    for (int i = startingBlock + 1 ; i < getHeight() ; i++)
    {
        msg.json()["blockIndexes"].push_back(i);
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
        case MsgTypes::Inv:
            handleInv(connection, msg);
            break;
        case MsgTypes::GetData:
            handleGetData(connection, msg);
            break;
        case MsgTypes::Block:
            handleBlock(connection, msg);
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
    verack(connection);
}

void keyser::Node::handleVerack(std::shared_ptr<Connection> connection, Message& msg)
{    
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
    Transaction transaction;
    msg.extract(transaction);

    if (_mempool.addTransaction(transaction))
        messageNeighbors(msg, connection);
}

void keyser::Node::handleDistributeBlock(std::shared_ptr<Connection> connection, Message& msg)
{
    Block block;
    msg.extract(block);

    if (_validationEngine->validateBlock(block))
        messageNeighbors(msg, connection);
}

void keyser::Node::handleGetBlocks(std::shared_ptr<Connection> connection, Message& msg)
{   
    msg.deserialize();
    std::string topBlockHash = msg.json()["topBlock"];

    int topBlockI = 0;

    for (auto& block : _blocks)
    {   
        if (block->_hash == topBlockHash)
            break;

        topBlockI++;
    }

    inv(connection, topBlockI);
}

void keyser::Node::handleInv(std::shared_ptr<Connection> connection, Message& msg)
{
    msg.deserialize();

    if (msg.json()["blockIndexes"].size() == 0)
    {
        completedInitialBlockDownload();
        return;
    }

    for (int i : msg.json()["blockIndexes"])
    {
        _inventory.push_back(i);
    }

    getData(); 
}

void keyser::Node::handleGetData(std::shared_ptr<Connection> connection, Message& msg)
{
    msg.deserialize();

    for (auto i : msg.json()["blockIndexes"])
    {
        Message msg(MsgTypes::Block);
        msg.insert(*_blocks.at(i));
        message(connection, msg);
    }
}

void keyser::Node::handleBlock(std::shared_ptr<Connection> connection, Message& msg)
{
    Block block;
    msg.extract(block);
    _validationEngine->validateBlock(block);

    if (getHeight() >= _inventory.back() + 1)
    {
        completedInitialBlockDownload();
    }
}

void keyser::Node::handleGetNodeList(std::shared_ptr<Connection> connection, Message& msg)
{
    nodeInfoStream(connection);
}

void keyser::Node::handleNodeInfo(std::shared_ptr<Connection> connection, Message& msg)
{
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