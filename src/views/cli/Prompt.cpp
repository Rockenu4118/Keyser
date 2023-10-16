#include <iostream>
#include <iomanip>

#include "./include/Prompt.hpp"
#include "../../chain/include/WalletCollection.hpp"


void Keyser::cli::promptNetConfig(int& serverPort, int& clientPort)
{
    std::cout << "Server Port: ";
    std::cin  >> serverPort;
    std::cout << "Client Port: ";
    std::cin  >> clientPort;
}

void Keyser::cli::promptMainMenu(int& selection, bool& miningStatus)
{
    system("clear");
    std::cout << "Mining: " << (miningStatus ? "ON" : "OFF") << std::endl;
    std::cout << std::endl;
    
    std::cout << "Menu" << std::endl;
    std::cout << std::setfill('-') << std::setw(50);
    std::cout << "-" << std::endl;

    std::cout << "[1] Begin mining"         << std::endl;
    std::cout << "[2] Create transaction"   << std::endl;
    std::cout << "[3] View Wallets"         << std::endl;
    std::cout << "[0] Exit"                 << std::endl;
    std::cout << std::endl;

    std::cout << "Selection: ";
    std::cin  >> selection;
}

void Keyser::cli::promptTransactionMenu()
{   
    system("clear");
    std::cout << "Creating Transaction" << std::endl;
    std::cout << std::setfill('-') << std::setw(50);
    std::cout << "-" << std::endl;

    std::cout << "\nPress any key to continue...";
    char c;
    std::cin >> c;
}

void Keyser::cli::promptWalletMenu(WalletCollection& wallets)
{   
    system("clear");
    std::cout << "Wallets" << std::endl;
    std::cout << std::setfill('-') << std::setw(50);
    std::cout << "-" << std::endl;

    wallets.displayWallets();

    std::cout << "\nPress any key to continue...";
    char c;
    std::cin >> c;
}