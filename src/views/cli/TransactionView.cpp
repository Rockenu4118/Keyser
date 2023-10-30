#include <iostream>
#include <iomanip>

#include "./TransactionView.hpp"
#include "../../chain/WalletManager.hpp"


keyser::cli::TransactionView::TransactionView(WalletManager& wallets) : _wallets(wallets)
{
    display();
}

void keyser::cli::TransactionView::display()
{
    int         amount;
    std::string recievingAddress;
    std::string sendingPublicKey;

    // system("clear");
    std::cout << "Creating Transaction" << std::endl;
    std::cout << std::setfill('-') << std::setw(50);
    std::cout << "-" << std::endl;

    std::cout << "Recieving address: ";
    std::cin  >> recievingAddress;

    std::cout << "Sending public key: ";
    std::cin  >> sendingPublicKey;

    std::cout << "Amount: ";
    std::cin  >> amount;

    std::cout << "\nPress any key to continue...";
    char c;
    std::cin >> c;
}