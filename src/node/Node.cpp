#include <iostream>

#include "./Node.hpp"
#include "../chain/Chain.hpp"
#include "../chain/Block.hpp"
#include "../chain/Transaction.hpp"
#include "../net/Server.hpp"
#include "../net/Client.hpp"


keyser::node::Node::Node(int serverPort, bool& miningStatus) : _miningStatus(miningStatus)
{
    _chain  = new keyser::Chain(4, 100);
    _server = new net::Server(serverPort, _chain);
    _client = new net::Client();      
}

keyser::node::Node::~Node()
{
    stop();
}

void keyser::node::Node::start(int clientPort)
{
    _server->start();
    _client->connect("127.0.0.1", clientPort);

    _serverResponseThr = std::thread([this]() { updateServerMessages(); });
    _clientResponseThr = std::thread([this]() { updateClientMessages(); });     
}

void keyser::node::Node::stop()
{}

void keyser::node::Node::sendTransaction(Transaction& transaction)
{
    _client->sendTransaction(transaction);
}

void keyser::node::Node::sendBlock(Block& block)
{
    _client->sendBlock(block);
}

void keyser::node::Node::ping()
{
    _client->ping();
}

void keyser::node::Node::messageAll()
{
    _client->messageAll();
}

void keyser::node::Node::updateServerMessages()
{
    while (1) {
        _server->update();
    }
}

void keyser::node::Node::updateClientMessages()
{
    while (1)
    {
        _client->update();
    }
}

void keyser::node::Node::beginMining(bool continuous)
{
    if (_miningStatus == true)
    {
        std::cout << "[CHAIN] Mining in progress." << std::endl;
        return;
    }

    _miningStatus = true;

    if (continuous)
        _miningThr = std::thread([this]() { mineContinuously(); });
    else
        _miningThr = std::thread([this]() { mineSingleBlock(); });

    _miningThr.detach();
}

void keyser::node::Node::mineContinuously()
{
    while (1)
    {
        _chain->mineBlock("aj");
        std::cout << "[CHAIN] block mined." << std::endl;
        _client->sendBlock(*_chain->getCurrBlock());
    }
}

void keyser::node::Node::mineSingleBlock()
{
    _chain->mineBlock("aj");
    std::cout << "[CHAIN] block mined." << std::endl;
    _client->sendBlock(*_chain->getCurrBlock());
    _miningStatus = false;
}

keyser::Chain* keyser::node::Node::chain()
{
    return _chain;
}