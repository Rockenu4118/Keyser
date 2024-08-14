#include <iostream>
#include <iomanip>
#include <string>

#include "./NetworkView.hpp"
#include "../node/Node.hpp"
#include "../node/PeerInfo.hpp"


keyser::cli::NetworkView::NetworkView(Node* node)
{
    _node = node;

    display();
}

void keyser::cli::NetworkView::display()
{
    char selection;

    do
    {
        displayTitle("Network Menu");

        std::cout << "[1] View connections"     << std::endl;
        std::cout << "[2] View active nodes"    << std::endl;
        std::cout << "[3] Self Info"            << std::endl;
        std::cout << "[4] New connection"       << std::endl;
        std::cout << "[5] Start server"         << std::endl;
        std::cout << "[0] Exit"                 << std::endl;
        std::cout << std::endl;

        promptSelection(selection);

        switch (selection)
        {
            case '1':
                displayTitle("Connections");
                _node->network()->displayPeers();
                continueMsg();
                break;
            case '2':
                displayTitle("Active Nodes");
                _node->network()->displayListeningNodes();
                continueMsg();
                break;
            case '3':
                displayTitle("Self Info");
                _node->network()->displaySelfInfo();
                continueMsg();
                break;
            case '4':
                newConnection();
                continueMsg();
                break;
            case '5':
                _node->network()->startServer();
                continueMsg();
                break;
            default:
                std::cout << "Invalid selection." << std::endl;
                break;
        }

        clearScreen();
    }
    while (selection != '0');
}

void keyser::cli::NetworkView::newConnection()
{
    Endpoint endpoint;
    endpoint.address = "127.0.0.1";

    std::cout << "Ip: ";
    std::cin >> endpoint.address;
    std::cout << "Port: ";
    std::cin >> endpoint.port;

    _node->network()->client()->connect(endpoint);
}