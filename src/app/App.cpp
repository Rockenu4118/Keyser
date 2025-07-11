#include "./App.hpp"

keyser::App::App()
{}

void keyser::App::init(int argc, char* argv[])
{
    int port = 6000;

    if (argc == 2)
    {
        port = std::stoi(argv[1]);
    }

    // Startup node
    mNode = new Node();
    mMenu = new MainMenu(mNode);

    // mNode->mChain = std::make_shared<Chain>();
    mNode->mNetwork = std::make_shared<NetInterface>(mNode, mMenu, port);


    mMenu->start();

    delete mNode;
    delete mMenu;
}