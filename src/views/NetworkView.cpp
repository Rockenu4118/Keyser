#include <iostream>
#include <iomanip>
#include <string>

#include "./NetworkView.hpp"
#include "../node/Node.hpp"
#include "../node/NodeInfo.hpp"


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
        std::cout << "[0] Exit"                 << std::endl;
        std::cout << std::endl;

        promptSelection(selection);

        switch (selection)
        {
            case '1':
                displayTitle("Connections");
                _node->displayConnections();
                continueMsg();
                break;
            case '2':
                displayTitle("Active Nodes");
                _node->displayActiveNodes();
                continueMsg();
                break;
            case '3':
                displayTitle("Self Info");
                _node->displaySelfInfo();
                continueMsg();
                break;
            case '4':
                newConnection();
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
    NodeInfo nodeInfo;
    nodeInfo._address = "127.0.0.1";

    std::cout << "Ip: ";
    std::cin >> nodeInfo._address;
    std::cout << "Port: ";
    std::cin >> nodeInfo._port;

    _node->connect(nodeInfo);
}