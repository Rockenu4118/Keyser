#include <iostream>
#include <iomanip>

#include "./ChainView.hpp"
#include "../../node/Node.hpp"


keyser::cli::ChainView::ChainView(node::Node* node, bool& miningStatus) : _miningStatus(miningStatus)
{
    _node = node;

    display();
}

void keyser::cli::ChainView::display()
{
    char selection;

    displayTitle("Chain Menu");

    do
    {
        std::cout << "[1] View chain"   << std::endl;
        std::cout << "[2] Begin mining" << std::endl;
        std::cout << "[0] Exit"         << std::endl;
        std::cout << std::endl;

        promptSelection(selection);

        switch (selection)
        {
            case '1':
                _node->printChain();
                continueMsg();
                break;
            case '2':
                _node->beginMining();
                break;
            default:
                std::cout << "Invalid selection." << std::endl;
                break;
        }
    }
    while (selection != '0');
}