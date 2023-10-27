#include <iostream>

#include "./Node.hpp"
#include "../chain/Chain.hpp"
#include "../net/Server.hpp"
#include "../net/Client.hpp"


keyser::Node::Node(NodeType nodeType, int port = 6000)
{
    _nodeType = nodeType;

    switch (_nodeType)
    {
        case NodeType::FullNode:
            _chain  = new Chain(5, 100);
            _server = new Server(port);
            _client = new Client();
            break;
        case NodeType::WalletNode:
            _client = new Client();
            break;
    }
}

void keyser::Node::start()
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

void keyser::Node::ping()
{
    _client->ping();
}

void keyser::Node::updateServerMessages()
{
    while (1) {
        _server->update();
    }
}

void keyser::Node::updateClientMessages()
{
    while (1)
    {
        _client->update();
    }
}

void keyser::Node::beginMining()
{
    _miningThr = std::thread([this]() { miningSequence(); });
}

void keyser::Node::miningSequence()
{
    _chain->createGenesisBlock();

    while (1)
    {
        _chain->mineBlock("aj");
        std::cout << *_chain->getCurrBlock() << std::endl;
    }
}