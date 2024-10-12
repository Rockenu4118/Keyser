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

        std::cout << "[1] View chain"              << std::endl;
        std::cout << "[2] View headers"            << std::endl; 
        std::cout << "[3] View block transactions" << std::endl;
        std::cout << "[4] View mempool"            << std::endl;
        std::cout << "[5] View Utxo set"           << std::endl;
        std::cout << "[6] Mine continuously"       << std::endl;
        std::cout << "[7] Mine single block"       << std::endl;
        std::cout << "[8] Stop mining"             << std::endl;
        std::cout << "[0] Exit"                    << std::endl;
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
                displayTitle("Headers");
                _node->chain()->printHeaders();
                continueMsg();
                break;
            case '3':
                viewBlockTransactions();
                break;
            case '4':
                displayTitle("Mempool");
                _node->mempool()->printMempool();
                continueMsg();
                break;
            case '5':
                displayTitle("Utxo Set");
                _node->txoSet()->printTxoSet();
                continueMsg();
                break;
            case '6':
                _node->beginMining();
                break;
            case '7':
                _node->beginMining(1);
                break;
            case '8':
                _node->stopMining();
                break;
            default:
                std::cout << "Invalid selection." << std::endl;
                break;
        }

        clearScreen();
    }
    while (selection != '0');
}

void keyser::cli::ChainView::viewBlockTransactions()
{
    int index;

    std::cout << "Block #: ";
    std::cin >> index;

    std::cout << std::endl;

    if (index < _node->chain()->blocks().size())
        _node->chain()->getBlock(index).printTransactions();
    else
        std::cout << "Invalid block." << std::endl;
        
    continueMsg();
}