#include "./App.hpp"

keyser::App::App() : mNode(nullptr)
{}

void keyser::App::run()
{
    // Prompt for port to run on
    // int  port;
    // char selection;

    // std::cout << "Port: ";
    // std::cin  >> port;
    
    // Startup node
    mNode = new Node(6000);
    mNode->run();

    // Startup RPC API
    // _rpc = new keyser::RPC(_node, port + 2080);
    // _rpc->run();

    // Initialize CLI
    // _view = new cli::RootView(_node);
    mMenu = new MainMenu(mNode);
    mMenu->start();

    delete mNode;
    delete mMenu;
    // delete _rpc;
}