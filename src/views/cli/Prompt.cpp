#include <iostream>
#include <iomanip>
#include <string>

#include "./Prompt.hpp"
#include "../../chain/WalletManager.hpp"
#include "../../node/Node.hpp"


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

    std::cout << "[1] Chain Menu"           << std::endl;
    std::cout << "[2] Create transaction"   << std::endl;
    std::cout << "[3] View Wallets"         << std::endl;
    std::cout << "[4] Ping"                 << std::endl;
    std::cout << "[0] Exit"                 << std::endl;
    std::cout << std::endl;

    std::cout << "Selection: ";
    std::cin  >> selection;
}
