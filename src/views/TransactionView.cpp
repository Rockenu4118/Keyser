#include <iostream>
#include <iomanip>
#include <vector>

#include "./TransactionView.hpp"
#include "../chain/Transaction.hpp"
#include "../wallet/Wallet.hpp"


// keyser::cli::TransactionView::TransactionView(Node* node)
// {
//     _node = node;
//
//     display();
// }
//
// void keyser::cli::TransactionView::display()
// {
//     char selection;
//
//     do
//     {
//         displayTitle("Transaction Menu");
//
//         std::cout << "[1] New Transaction" << std::endl;
//         std::cout << "[0] Exit"            << std::endl;
//         std::cout << std::endl;
//
//         promptSelection(selection);
//
//         switch (selection)
//         {
//             case '1':
//                 newTransaction();
//                 continueMsg();
//                 break;
//             default:
//                 std::cout << "Invalid selection." << std::endl;
//                 break;
//         }
//
//         // clearScreen();
//     }
//     while(selection != '0');
// }
//
// void keyser::cli::TransactionView::newTransaction()
// {
//     std::string name;
//     double      amount;
//     std::string recipient;
//     char        confirmation;
//
//     displayTitle("Creating Transaction");
//     displayTitle("Wallets");
//
//     _node->wallet()->displayAccounts();
//
//     promptInput("Recieving address: ", recipient);
//     promptInput("Sending wallet: ", name);
//     promptInput("Amount: ", amount);
//
//     promptConfirmation(confirmation);
//
//     if (confirmation != 'y')
//         return;
//
//     bool success = _node->wallet()->createTransaction(amount, recipient, _node->wallet()->get(name));
//
//     if (!success)
//         std::cout << "Did not succeed!" << std::endl;
// }