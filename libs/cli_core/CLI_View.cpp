#include <iostream>
#include <iomanip>
#include <string>

#include "./CLI_View.hpp"


void cli_core::CLI_View::display()
{}

void cli_core::CLI_View::displayTitle(std::string title)
{
    std::cout << title << std::endl;
    std::cout << std::setfill('-') << std::setw(50);
    std::cout << "-" << std::endl;
}

void cli_core::CLI_View::promptSelection(char& selection)
{
    std::cout << "Selection: ";
    std::cin  >> selection;
}

void cli_core::CLI_View::promptConfirmation(char& confirmation)
{
    std::cout << "Confirm? (y/n): ";
    std::cin  >> confirmation;
}

void cli_core::CLI_View::promptInput(std::string msg, std::string& input, bool extraEndl)
{
    std::cout << msg;
    std::cin  >> input;

    if (extraEndl)
        std::cout << std::endl;
}

void cli_core::CLI_View::promptInput(std::string msg, double& input, bool extraEndl)
{
    std::cout << msg;
    std::cin  >> input;

    if (extraEndl)
        std::cout << std::endl;
}

void cli_core::CLI_View::promptInput(std::string msg, uint& input, bool extraEndl)
{
    std::cout << msg;
    std::cin  >> input;

    if (extraEndl)
        std::cout << std::endl;
}

void cli_core::CLI_View::clearScreen()
{
    system("clear");
}

void cli_core::CLI_View::continueMsg()
{
    char c;
    std::cout << std::endl;
    std::cout << "Press any key to continue...";
    std::cin  >> c;
    std::cout << std::endl;
}