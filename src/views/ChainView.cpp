#include <iostream>
#include <iomanip>

#include "./ChainView.hpp"
#include "../node/Node.hpp"


keyser::cli::ChainView::ChainView(Node* node)
{
    _node = node;

    display();
}

void keyser::cli::ChainView::display()
{
    char selection;

    do
    {
        displayTitle("Chain Menu");

        std::cout << "[1] View chain"        << std::endl;
        std::cout << "[2] View mempool"      << std::endl;  
        std::cout << "[3] Mine continuously" << std::endl;
        std::cout << "[4] Mine single block" << std::endl;
        std::cout << "[0] Exit"              << std::endl;
        std::cout << std::endl;

        promptSelection(selection);

        switch (selection)
        {
            case '1':
                displayTitle("Chain");
                _node->chain()->printChain();
                continueMsg();
                break;
            case '2':
                displayTitle("Mempool");
                _node->chain()->mempool()->printMempool();
                continueMsg();
                break;
            case '3':
                _node->beginMining(true);
                break;
            case '4':
                _node->beginMining(false);
                break;
            default:
                std::cout << "Invalid selection." << std::endl;
                break;
        }

        clearScreen();
    }
    while (selection != '0');
}