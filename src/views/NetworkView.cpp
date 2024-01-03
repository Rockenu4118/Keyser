#include <iostream>
#include <iomanip>
#include <string>

#include "./NetworkView.hpp"
#include "../node/Node.hpp"


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

        std::cout << "[1] View connections" << std::endl;
        std::cout << "[2] New connection"   << std::endl;
        std::cout << "[0] Exit"             << std::endl;
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
    std::string ip = "127.0.0.1";
    uint16_t    port;

    // std::cout << "Ip: ";
    // std::cin >> ip;
    std::cout << "Port: ";
    std::cin >> port;

    _node->connect(ip, port);
}