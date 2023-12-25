#include <iostream>
#include <iomanip>

#include "./NetworkView.hpp"
#include "../../node/Node.hpp"


keyser::cli::NetworkView::NetworkView(Node* node)
{
    _node = node;

    display();
}

void keyser::cli::NetworkView::display()
{
    char selection;

    displayTitle("Network Menu");

    do
    {
        std::cout << "[1] View connections" << std::endl;
        std::cout << "[2] New connection"   << std::endl;
        std::cout << "[0] Exit"             << std::endl;
        std::cout << std::endl;

        promptSelection(selection);

        switch (selection)
        {
            case '1':
                _node->displayConnections();
                continueMsg();
                break;
            case '2':
                newConnection();
                continueMsg();
                break;
            default:
                std::cout << "Invalid selection." << std::endl;
                break;
        }
    }
    while (selection != '0');

    clearScreen();
}

void keyser::cli::NetworkView::newConnection()
{
    uint16_t port;

    std::cout << "Port: ";
    std::cin >> port;

    _node->connect("192.168.56.1", port);
}