#include <iostream>
#include <iomanip>

#include "./TransactionView.hpp"
#include "../../chain/Transaction.hpp"
#include "../../chain/WalletManager.hpp"


keyser::cli::TransactionView::TransactionView(WalletManager& wallets, node::Node* node) : _wallets(wallets)
{
    _node = node;

    display();
}

void keyser::cli::TransactionView::display()
{
    double      amount;
    std::string recievingAddress;
    std::string sendingPublicKey;

    char confirmation;

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

    std::cout << "Confirm? (y/n): ";
    std::cin  >> confirmation;

    if (confirmation == 'y')
    {
        Transaction transaction = Transaction(amount, recievingAddress, sendingPublicKey);
        _node->sendTransaction(transaction);
    }
}