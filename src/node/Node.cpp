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
#include "../wallet/Wallet.hpp"


keyser::Node::Node(uint16_t port)
{
    _startTime = time(NULL);

    _chain            = std::make_shared<Chain>(this);
    _mempool          = std::make_shared<Mempool>(this);
    _utxoSet          = std::make_shared<UtxoSet>(this);
    _network          = std::make_shared<NetInterface>(this, port);
    _validationEngine = std::make_shared<ValidationEngine>(this);
    _wallet           = std::make_shared<Wallet>(this);
}

keyser::Node::~Node()
{
    shutdown();
}

void keyser::Node::run()
{
    _chain->createGenesisBlock();
}

void keyser::Node::shutdown()
{
    stopMining();

    _shutdownNode = true;
}

keyser::Node::Status keyser::Node::status() const
{
    return _status;
}

void keyser::Node::status(Status status)
{
    _status = status;
}

time_t keyser::Node::getUptime() const
{
    return time(NULL) - _startTime;
}

bool keyser::Node::shutdownFlag() const
{
    return _shutdownNode;
}

void keyser::Node::beginMining(uint numBlocks)
{
    if (_miner.use_count() == 0)
    {
        _miner = std::make_shared<Miner>(this);
    }
    
    _miner->start(numBlocks);
}

void keyser::Node::stopMining()
{
    if (_miner.use_count() == 1)
    {
        _miner.reset();
    }
}

bool keyser::Node::submitTransaction(Transaction transaction)
{
    if (_validationEngine->validateTransaction(transaction))
    {
        _network->distributeTransaction(transaction);
        return true;
    }
    return false;
}

std::shared_ptr<keyser::Chain>& keyser::Node::chain()
{
    return _chain;
}

std::shared_ptr<keyser::Miner>& keyser::Node::miner()
{
    return _miner;
}

std::shared_ptr<keyser::Mempool>& keyser::Node::mempool()
{
    return _mempool;
}

std::shared_ptr<keyser::UtxoSet>& keyser::Node::utxoSet()
{
    return _utxoSet;
}

std::shared_ptr<keyser::NetInterface>& keyser::Node::network()
{
    return _network;
}

std::shared_ptr<keyser::ValidationEngine>& keyser::Node::validationEngine()
{
    return _validationEngine;
}

std::shared_ptr<keyser::Wallet>& keyser::Node::wallet()
{
    return _wallet;
}
