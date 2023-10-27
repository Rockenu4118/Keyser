#include <iostream>
#include <iomanip>
#include <string>

#include "./Prompt.hpp"
#include "../../chain//WalletCollection.hpp"
#include "../../node//Node.hpp"


void keyser::cli::promptNetConfig(int& serverPort, int& clientPort)
{
    std::cout << "Server Port: ";
    std::cin  >> serverPort;
    std::cout << "Client Port: ";
    std::cin  >> clientPort;
}

void keyser::cli::promptMainMenu(char& selection, bool& miningStatus)
{
    // system("clear");
    std::cout << "Mining: " << (miningStatus ? "ON" : "OFF") << std::endl;
    std::cout << std::endl;
    
    std::cout << "Menu" << std::endl;
    std::cout << std::setfill('-') << std::setw(50);
    std::cout << "-" << std::endl;

    std::cout << "[1] Chain Menu"         << std::endl;
    std::cout << "[2] Create transaction"   << std::endl;
    std::cout << "[3] View Wallets"         << std::endl;
    std::cout << "[4] Ping"                 << std::endl;
    std::cout << "[0] Exit"                 << std::endl;
    std::cout << std::endl;

    std::cout << "Selection: ";
    std::cin  >> selection;
}

void keyser::cli::promptMiningMenu(keyser::Node* node, bool& miningStatus)
{
    char selection;

    std::cout << "Mining status: " << (miningStatus ? "ON" : "OFF") << std::endl;
    std::cout << std::endl;

    std::cout << "Mining" << std::endl;
    std::cout << std::setfill('-') << std::setw(50);
    std::cout << "-" << std::endl;

    if (!miningStatus)
    {
        std::cout << "Begin mining? (y/n): ";
        std::cin >> selection;

        if (selection == 'y')
        {
            node->beginMining();
            miningStatus = true;
        }
    }
    else 
    {
        std::cout << "Quit mining? (y/n): ";
        std::cin >> selection;
        
        // TODO - Stop mining
    }
}

void keyser::cli::promptTransactionMenu(keyser::Node* node)
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

void keyser::cli::promptWalletMenu(WalletCollection& wallets)
{   
    // system("clear");
    std::cout << "Wallets" << std::endl;
    std::cout << std::setfill('-') << std::setw(50);
    std::cout << "-" << std::endl;

    wallets.displayWallets();

    std::cout << "\nPress any key to continue...";
    char c;
    std::cin >> c;
}