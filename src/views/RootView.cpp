#include <iostream>
#include <vector>

#include "./RootView.hpp"
#include "./ChainView.hpp"
#include "./TransactionView.hpp"
#include "./WalletView.hpp"
#include "./NetworkView.hpp"
#include "../node/Node.hpp"
#include "../wallet/WalletManager.hpp"


keyser::cli::RootView::RootView(Node* node, WalletManager& wallets) : _wallets(wallets)
{
    _node = node;

    display();
}

void keyser::cli::RootView::display()
{
    initSetup();

    char selection;

    do
    {
        displayTitle("Keyser Protocol");

        std::cout << "[1] Chain Menu"         << std::endl;
        std::cout << "[2] Create transaction" << std::endl;
        std::cout << "[3] View Wallets"       << std::endl;
        std::cout << "[4] Ping"               << std::endl;
        std::cout << "[5] Network Menu"       << std::endl;
        std::cout << "[0] Exit"               << std::endl;
        std::cout << std::endl;

        promptSelection(selection);
        clearScreen();

        switch (selection)
        {
            case '1':
            {
                keyser::cli::ChainView view = keyser::cli::ChainView(_node);
            }
                break;
            case '2':
            {
                keyser::cli::TransactionView view = keyser::cli::TransactionView(_wallets, _node);
            }
                break;  
            case '3':
            {
                keyser::cli::WalletView view = keyser::cli::WalletView(_wallets);
            }   
                break;
            case '4':
            {
                _node->ping();
            }
                break;
            case '5':
            {
                keyser::cli::NetworkView view = keyser::cli::NetworkView(_node);
            }
                break;
            default:
                std::cout << "Exiting program..." << std::endl;
                break;
        }

        clearScreen();
    }
    while (selection != '0');
}

void keyser::cli::RootView::initSetup()
{
    displayTitle("Keyser Protocol Initial Connection");

    char selection;

    std::cout << "Establish init connect (y/n): ";
    std::cin >> selection;

    if (selection == 'y') 
    {
        initConnection();
    }
}

void keyser::cli::RootView::initConnection()
{
    bool success = false;

    do
    {
        NodeInfo nodeInfo;
        nodeInfo._address = "127.0.0.1";

        std::cout << "Ip: ";
        std::cin >> nodeInfo._address;
        std::cout << "Port: ";
        std::cin >> nodeInfo._port;

        success = _node->connect(nodeInfo);
    }
    while (!success);
}