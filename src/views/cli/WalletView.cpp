#include <iostream>
#include <iomanip>

#include "./WalletView.hpp"
#include "../../wallet/WalletManager.hpp"


keyser::cli::WalletView::WalletView(WalletManager& wallets) : _wallets(wallets)
{
    display();
}

void keyser::cli::WalletView::display()
{
    char selection;

    displayTitle("Wallet Menu");

    do
    {
        std::cout << "[1] View wallets"  << std::endl;
        std::cout << "[2] Edit wallet"   << std::endl;
        std::cout << "[3] Create wallet" << std::endl;
        std::cout << "[0] Exit"          << std::endl;
        std::cout << std::endl;

        promptSelection(selection);

        switch (selection)
        {
            case '1':
                _wallets.displayWallets();
                continueMsg();
                break;
            case '2':
                // TODO
                break;
            case '3':
                // TODO
                break;
            default:
                std::cout << "Invalid selection." << std::endl;
                break;
        }
    }
    while (selection != '0');

    clearScreen();
}