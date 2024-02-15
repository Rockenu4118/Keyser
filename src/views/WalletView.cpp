#include <iostream>
#include <iomanip>

#include "./WalletView.hpp"
#include "../node/Node.hpp"


keyser::cli::WalletView::WalletView(Node* node)
{
    _node = node;

    display();
}

void keyser::cli::WalletView::display()
{
    char selection;

    do
    {
        displayTitle("Wallet Menu");

        std::cout << "[1] View wallets"  << std::endl;
        std::cout << "[2] Edit wallet"   << std::endl;
        std::cout << "[3] Create wallet" << std::endl;
        std::cout << "[0] Exit"          << std::endl;
        std::cout << std::endl;

        promptSelection(selection);

        switch (selection)
        {
            case '1':
                viewWallets();
                continueMsg();
                break;
            case '2':
                // TODO
                break;
            case '3':
                createWallet();
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

void keyser::cli::WalletView::viewWallets()
{
    displayTitle("Wallets");
    _node->walletManager()->displayWallets();
}

void keyser::cli::WalletView::createWallet()
{
    std::string name;

    displayTitle("New Wallet");
    std::cout << "Name: ";
    std::cin >> name;
    _node->walletManager()->createWallet(name);
}