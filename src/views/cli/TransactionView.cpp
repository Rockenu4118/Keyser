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
    char selection;

    displayTitle("Transaction Menu");

    do
    {
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
    }
    while(selection != '0');

    clearScreen();
}

void keyser::cli::TransactionView::newTransaction()
{
    uint        walletIndex;
    double      amount;
    std::string recievingAddress;
    Wallet      sendingWallet;
    char        confirmation;

    displayTitle("Creating Transaction");

    promptInput("Recieving address: ", recievingAddress);

    std::cout << "Wallets:" << std::endl;

    _wallets.displayWallets();

    promptInput("Sending wallet: ", walletIndex);

    _wallets.getWallet(sendingWallet, walletIndex);

    if (sendingWallet.getKeyPair() == nullptr)
        return;

    promptInput("Amount: ", amount);

    std::cout << "Amount, "           << amount 
              << ", Reciever: "       << recievingAddress 
              << ", Sending wallet: " << sendingWallet.getName() 
              << std::endl;

    promptConfirmation(confirmation);

    if (confirmation == 'y')
    {
        Transaction transaction = Transaction(amount, recievingAddress, sendingWallet.getKeyPair()->getUPublicKey());
        transaction.sign(sendingWallet.getKeyPair());
        _node->sendTransaction(transaction);
    }
}