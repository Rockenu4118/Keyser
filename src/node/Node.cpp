#include <iostream>

#include "./Node.hpp"
#include "../chain/Chain.hpp"
#include "../chain/Block.hpp"
#include "../chain/Transaction.hpp"
#include "../net/Server.hpp"
#include "../net/Client.hpp"


keyser::node::Node::Node(NodeType nodeType, int port = 6000)
{
    _nodeType = nodeType;

    switch (_nodeType)
    {
        case NodeType::FullNode:
            _chain  = new keyser::Chain(5, 100);
            _server = new net::Server(port);
            _client = new net::Client();
            break;
        case NodeType::WalletNode:
            _client = new net::Client();
            break;
    }
}

void keyser::node::Node::start()
{
    switch (_nodeType)
    {
        case NodeType::FullNode:
            _server->start();
            _client->connect("127.0.0.1", 6000);

            _serverResponseThr = std::thread([this]() { updateServerMessages(); });
            _clientResponseThr = std::thread([this]() { updateClientMessages(); });
            break;
        case NodeType::WalletNode:
            _client->connect("127.0.0.1", 6000);

            _clientResponseThr = std::thread([this]() { updateClientMessages(); });
            break;
    }
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
    _miningThr = std::thread([this]() { miningSequence(); });
}

void keyser::node::Node::miningSequence()
{
    _chain->createGenesisBlock();

    while (1)
    {
        _chain->mineBlock("aj");
        std::cout << *_chain->getCurrBlock() << std::endl;
    }
}