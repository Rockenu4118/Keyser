#include <iostream>
#include <iomanip>
#include <vector>

#include "./TransactionView.hpp"
#include "../chain/Transaction.hpp"
#include "../wallet/WalletManager.hpp"


keyser::cli::TransactionView::TransactionView(Node* node)
{
    _node = node;

    newTransaction();
    clearScreen();
}

void keyser::cli::TransactionView::display()
{
    char selection;

    do
    {
        displayTitle("Transaction Menu");

        std::cout << "[1] New Transaction" << std::endl;
        std::cout << "[0] Exit"            << std::endl;
        std::cout << std::endl;

        promptSelection(selection);

        switch (selection)
        {
            case '1':
                newTransaction();
                continueMsg();
                break;
            default:
                std::cout << "Invalid selection." << std::endl;
                break;
        }

        clearScreen();
    }
    while(selection != '0');
}

void keyser::cli::TransactionView::newTransaction()
{
    uint        index;
    double      amount;
    std::string msg;
    std::string recievingAddress;
    char        confirmation;

    displayTitle("Creating Transaction");
    displayTitle("Wallets");

    _node->walletManager().displayWallets();

    promptInput("Recieving address: ", recievingAddress);
    promptInput("Sending wallet: ", index);
    
    if (index >= _node->walletManager().count())
        return;

    promptInput("Amount: ", amount);

    Transaction transaction = Transaction(amount, msg, recievingAddress, _node->walletManager().at(index).getKeyPair()->getUPublicKey());
    transaction.sign(_node->walletManager().at(index).getKeyPair());
    
    std::cout << transaction << std::endl;
    
    promptConfirmation(confirmation);

    if (confirmation != 'y')
        return;

    bool success = _node->createTransaction(transaction);

    if (!success)
        std::cout << "Insufficient balance!" << std::endl;
}