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


keyser::Node::Node(uint16_t port) : NetInterface(port, _shutdownNode)
{
    _startTime = time(NULL);

    // Test wallets
    keyser::Wallet ajWallet("AJ", key1);
    _walletManager.addWallet(ajWallet);

    keyser::Wallet guyWallet("Guy", key2);
    _walletManager.addWallet(guyWallet);

    _validationEngine = new ValidationEngine(this);
}

keyser::Node::~Node()
{
    shutdown();
}

void keyser::Node::run()
{
    boost::asio::io_context::work idleWork(_context);
    
    _contextThread = std::thread([this]() { _context.run(); });  

    // Run thread to handle messsages
    _responseThread = std::thread([this]() { update(); }); 

    startServer();
}

void keyser::Node::shutdown()
{
    stopMining();

    _shutdownNode = true;
}

keyser::Node::Status keyser::Node::getStatus() const
{
    return _status;
}

time_t keyser::Node::getUptime() const
{
    return time(NULL) - _startTime;
}

void keyser::Node::beginMining(uint numBlocks)
{
    
    if (_miningStatus == true)
    {
        std::cout << "[CHAIN] Mining In Progress" << std::endl;
        return;
    }

    if (_miningThr.joinable())
        _miningThr.join();

    _miningThr = std::thread([this, numBlocks]()
    {
        _miningStatus = true;

        uint blocksMined = 0;

        while (!_shutdownNode && _miningStatus && (blocksMined < numBlocks))
        {
            Block newBlock(getCurrBlock()->_index + 1, time(NULL), getCurrBlock()->_hash, popLeadingTransactions());

            while (1)
            {
                if (_shutdownNode || !_miningStatus)
                    return;

                if (newBlock.hasValidHash(calcDifficulty()))
                    break;

                newBlock._nonce++;
                newBlock._hash = newBlock.calcHash();
            }

            blocksMined++;

            std::cout << "[CHAIN] Block Mined." << std::endl;

            std::cout << _validationEngine->validateBlock(newBlock) << std::endl;

            distributeBlock(*getCurrBlock());
        }

        _miningStatus = false;
    });
}

void keyser::Node::stopMining()
{
    _miningStatus = false;

    if (_miningThr.joinable())
        _miningThr.join();
}

bool keyser::Node::createTransaction(Transaction& transaction)
{
    // TODO - create tx
    // double balance = getAddressBalance(transaction._senderAddress) + getPendingBalance(transaction._senderAddress);

    // if (transaction._amount > balance)
    //     return false;

    // _validationEngine->validateTransaction(transaction);
    // distributeTransaction(transaction);
    return true;
}

void keyser::Node::broadcastSelf()
{
    _activeNodeList.insert(_selfInfo);

    Message msg(MsgTypes::DistributeNodeInfo);
    msg.json() = _selfInfo.json();
    msg.preparePayload();
    messageNeighbors(msg);
}

void keyser::Node::completedInitialBlockDownload()
{
    _status = Status::Online;

    // Can now advertise self as a node on the network
    broadcastSelf();
}

void keyser::Node::ping()
{
    Message msg(MsgTypes::Ping);
    messageNeighbors(msg);
}

void keyser::Node::pong() {}

void keyser::Node::version(std::shared_ptr<Peer> peer)
{
    // Send self info as well as their external address
    Message msg(MsgTypes::Version);
    msg.json()["Outbound version"] = _selfInfo._version;
    msg.json()["Outbound alias"]   = _selfInfo._alias;
    msg.json()["Outbound port"]    = _selfInfo._port;
    msg.json()["chainHeight"]      = getHeight();
    msg.json()["address"]          = peer->getEndpoint().address().to_string();
    
    msg.preparePayload();
    message(peer, msg);
}

void keyser::Node::verack(std::shared_ptr<Peer> peer)
{
    Message newMsg(MsgTypes::Verack);
    newMsg.json()["Outbound version"] = _selfInfo._version;
    newMsg.json()["Outbound alias"]   = _selfInfo._alias;
    newMsg.json()["Outbound port"]    = _selfInfo._port;
    newMsg.json()["address"]          = peer->getEndpoint().address().to_string();

    newMsg.preparePayload();
    message(peer, newMsg);
}

void keyser::Node::getHeaders()
{
    Message msg(MsgTypes::GetHeaders);
    message(syncNode(), msg);
}

void keyser::Node::sendHeaders(std::shared_ptr<Peer> peer)
{
    // TODO - headers only
    Message msg(MsgTypes::Headers);


}

void keyser::Node::distributeNodeInfo(NodeInfo& nodeInfo)
{
    Message msg(MsgTypes::DistributeNodeInfo);
    msg.json() = nodeInfo.json();
    msg.preparePayload();
    messageNeighbors(msg);
}

void keyser::Node::distributeTransaction(Transaction& transaction)
{
    Message msg(MsgTypes::DistributeTransaction);
    msg.json() = transaction.json();
    msg.preparePayload();
    messageNeighbors(msg);
}

void keyser::Node::distributeBlock(Block& block)
{
    Message msg(MsgTypes::DistributeBlock);
    msg.json() = block.json();
    msg.preparePayload();
    messageNeighbors(msg);
}

void keyser::Node::getBlocks()
{
    Message msg(MsgTypes::GetBlocks);
    msg.json()["topBlock"] = getCurrBlock()->_hash;
    msg.preparePayload();

    message(syncNode(), msg);

    _status == Status::Syncing;
}

void keyser::Node::sendBlocks(std::shared_ptr<Peer> peer, int blockIndex)
{
    Message msg(MsgTypes::Block);
    msg.json() = _blocks.at(blockIndex)->json();
    msg.preparePayload();
    message(peer, msg);
}

void keyser::Node::getNodeList()
{
    Message msg(MsgTypes::GetNodeList);
    message(syncNode(), msg);
}

void keyser::Node::nodeInfoStream(std::shared_ptr<Peer> peer)
{
    Message msg(MsgTypes::NodeInfoList);

    msg.json() = nlohmann::json::array();

    for (auto& nodeInfo : _activeNodeList)
        msg.json().push_back(nodeInfo.json());

    msg.preparePayload();
    message(peer, msg);
}

void keyser::Node::getData()
{
    Message msg(MsgTypes::GetData);

    for (auto element : _inventory)
        msg.json()["blockIndexes"].push_back(element);

    msg.preparePayload();

    message(syncNode(), msg);
}

void keyser::Node::inv(std::shared_ptr<Peer> peer, int startingBlock)
{
    Message msg(MsgTypes::Inv);

    if (startingBlock == getHeight() - 1)
    {
        message(peer, msg);
        return;
    }
    
    for (int i = startingBlock + 1 ; i < getHeight() ; i++)
    {
        msg.json()["blockIndexes"].push_back(i);
    }

    msg.preparePayload();

    message(peer, msg);
}

keyser::WalletManager& keyser::Node::walletManager()
{
    return _walletManager;
}

bool keyser::Node::allowConnect(std::shared_ptr<Peer> peer)
{ return true; }

void keyser::Node::onOutgoingConnect(std::shared_ptr<Peer> peer)
{
    version(peer);
}

void keyser::Node::onIncomingConnect(std::shared_ptr<Peer> peer) 
{}

void keyser::Node::onDisconnect(std::shared_ptr<Peer> peer)
{}

void keyser::Node::onMessage(std::shared_ptr<Peer> peer, Message& msg)
{
    switch (msg.header.id)
    {
        case MsgTypes::Generic:
            std::cout << "[NODE] Generic msg" << std::endl;
            break;
        case MsgTypes::Ping:
            handlePing(peer, msg);
            break;
        case MsgTypes::DistributeNodeInfo:
            handleDistributeNodeInfo(peer, msg);
            break;
        case MsgTypes::DistributeBlock:
            handleDistributeBlock(peer, msg);
            break;
        case MsgTypes::DistributeTransaction:
            handleDistributeTransaction(peer, msg);
            break;
        case MsgTypes::Version:
            handleVersion(peer, msg);
            break;
        case MsgTypes::Verack:
            handleVerack(peer, msg);
            break;
        case MsgTypes::GetBlocks:
            handleGetBlocks(peer, msg);
            break;
        case MsgTypes::Inv:
            handleInv(peer, msg);
            break;
        case MsgTypes::GetData:
            handleGetData(peer, msg);
            break;
        case MsgTypes::Block:
            handleBlock(peer, msg);
            break;
        case MsgTypes::GetNodeList:
            handleGetNodeList(peer, msg);
            break;
        case MsgTypes::NodeInfoList:
            handleNodeInfo(peer, msg);
            break;
        case MsgTypes::GetHeaders:

            break;
        default:
            std::cout << "[NODE] Unknown Msg Type" << std::endl;
            break;
    }
}

void keyser::Node::handlePing(std::shared_ptr<Peer> peer, Message& msg)
{
    std::cout << utils::localTimestamp() << "Ping: " << peer->getId() << std::endl;
}

void keyser::Node::handleVersion(std::shared_ptr<Peer> peer, Message& msg)
{
    msg.unpackPayload();

    // Save external address and add self to list of active nodes
    _selfInfo._address = msg.json()["address"];
    _activeNodeList.insert(_selfInfo);

    // unpackPayload incoming node info
    peer->info()._version = msg.json()["Outbound version"];
    peer->info()._alias   = msg.json()["Outbound alias"];
    peer->info()._port    = msg.json()["Outbound port"];
    peer->info().startingHeight = msg.json()["chainHeight"];

    // Add this node to connectedNodeList
    _connectedNodeList.insert(peer->info());

    // Send self info as well as their external ip
    verack(peer);
}

void keyser::Node::handleVerack(std::shared_ptr<Peer> peer, Message& msg)
{    
    msg.unpackPayload();

    // Save external address
    _selfInfo._address = msg.json()["address"];

    // unpackPayload incoming node info
    peer->info()._version = msg.json()["Outbound version"];
    peer->info()._alias   = msg.json()["Outbound alias"];

    // Add info to connected node list and active node list
    _connectedNodeList.insert(peer->info());
    _activeNodeList.insert(peer->info());
    
    // Get block inv if needed
    if (!_blockInvRecieved)
        getBlocks();
    
    // Get node list if needed
    if (!_recievedNodeList)
        getNodeList();
}

void keyser::Node::handleDistributeNodeInfo(std::shared_ptr<Peer> peer, Message& msg)
{
    msg.unpackPayload();
    NodeInfo nodeInfo(msg.json());

    if (_activeNodeList.count(nodeInfo))
        return;

    _activeNodeList.insert(nodeInfo);
    _potentialConnections.push_back(nodeInfo);
    managePeers();

    messageNeighbors(msg, peer);
}

void keyser::Node::handleDistributeTransaction(std::shared_ptr<Peer> peer, Message& msg)
{
    msg.unpackPayload();
    Transaction transaction(msg.json());

    if (_validationEngine->validateTransaction(transaction))
        messageNeighbors(msg, peer);
}

void keyser::Node::handleDistributeBlock(std::shared_ptr<Peer> peer, Message& msg)
{
    msg.unpackPayload();
    Block block(msg.json());

    if (_validationEngine->validateBlock(block))
        messageNeighbors(msg, peer);
}

void keyser::Node::handleGetBlocks(std::shared_ptr<Peer> peer, Message& msg)
{   
    msg.unpackPayload();
    std::string topBlockHash = msg.json()["topBlock"];

    int topBlockI = 0;

    for (auto& block : _blocks)
    {   
        if (block->_hash == topBlockHash)
            break;

        topBlockI++;
    }

    inv(peer, topBlockI);
}

void keyser::Node::handleInv(std::shared_ptr<Peer> peer, Message& msg)
{
    msg.unpackPayload();

    _blockInvRecieved = true;

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

void keyser::Node::handleGetData(std::shared_ptr<Peer> peer, Message& msg)
{
    msg.unpackPayload();

    for (auto i : msg.json()["blockIndexes"])
    {
        Message msg(MsgTypes::Block);
        msg.json() = _blocks.at(i)->json();
        msg.preparePayload();
        message(peer, msg);
    }
}

void keyser::Node::handleBlock(std::shared_ptr<Peer> peer, Message& msg)
{
    msg.unpackPayload();
    Block block(msg.json());
    _validationEngine->validateBlock(block);

    if (getHeight() >= _inventory.back() + 1)
    {
        completedInitialBlockDownload();
    }
}

void keyser::Node::handleGetNodeList(std::shared_ptr<Peer> peer, Message& msg)
{
    nodeInfoStream(peer);
}

void keyser::Node::handleNodeInfo(std::shared_ptr<Peer> peer, Message& msg)
{
    msg.unpackPayload();

    for (auto& info : msg.json())
    {
        NodeInfo nodeInfo(info);
        _activeNodeList.insert(nodeInfo);
        _potentialConnections.push_back(nodeInfo);
    }

    managePeers();

    _recievedNodeList = true;
}

void keyser::Node::handleGetHeaders(std::shared_ptr<Peer> peer, Message& msg)
{

}

void keyser::Node::handleHeaders(std::shared_ptr<Peer> peer, Message& msg)
{

}