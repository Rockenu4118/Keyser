#include <iostream>

#include "./Node.hpp"
#include "../chain/Chain.hpp"
#include "../chain/Block.hpp"
#include "../chain/Transaction.hpp"
#include "../net/Server.hpp"
#include "../net/Client.hpp"


keyser::node::Node::Node(NodeType nodeType, int serverPort, bool& miningStatus) : _miningStatus(miningStatus)
{
    _nodeType = nodeType;

    switch (_nodeType)
    {
        case NodeType::FullNode:
            _chain  = new keyser::Chain(4, 100);
            _server = new net::Server(serverPort);
            _client = new net::Client();
            break;
        case NodeType::WalletNode:
            _client = new net::Client();
            break;
    }
}

keyser::node::Node::~Node()
{
    stop();
}

void keyser::node::Node::start(int clientPort)
{
    switch (_nodeType)
    {
        case NodeType::FullNode:
            _server->start();
            _client->connect("127.0.0.1", clientPort);

            _serverResponseThr = std::thread([this]() { updateServerMessages(); });
            _clientResponseThr = std::thread([this]() { updateClientMessages(); });
            break;
        case NodeType::WalletNode:
            _client->connect("127.0.0.1", clientPort);

            _clientResponseThr = std::thread([this]() { updateClientMessages(); });
            break;
    }
}

void keyser::node::Node::stop()
{
    if (_serverResponseThr.joinable());
        _serverResponseThr.join();
    
    if (_clientResponseThr.joinable());
        _clientResponseThr.join();
    
    if (_miningThr.joinable());
        _miningThr.join();
}

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

void keyser::node::Node::beginMining()
{
    if (_miningStatus == true)
    {
        std::cout << "[CHAIN] Mining in progress." << std::endl;
        return;
    }
    _miningStatus = true;

    _miningThr = std::thread([this]() { miningSequence(); });
    _miningThr.detach();
}

void keyser::node::Node::miningSequence()
{
    _chain->mineBlock("aj");
    std::cout << "[CHAIN] block mined." << std::endl;
    // _client->sendBlock(*_chain->getCurrBlock());
    _miningStatus = false;
}

void keyser::node::Node::printChain()
{
    _chain->printChain();
}