#include <iostream>

#include "./App.hpp"
#include "../views/RootView.hpp"
#include "../node/Node.hpp"
#include "../rpc/rpc.hpp"

keyser::App::App() : _node(nullptr), _rpc(nullptr), _view(nullptr)
{}

void keyser::App::run()
{
    // Prompt for port to run on
    int  port;
    char selection;

    std::cout << "Port: ";
    std::cin  >> port;
    
    // Startup node
    _node = new keyser::Node(port);
    _node->run();

    // Startup RPC API
    _rpc = new keyser::RPC(_node, port + 2080);
    _rpc->run();

    // delete _node;
    // delete _rpc;

    // Initialize CLI
    _view = new cli::RootView(_node);
}