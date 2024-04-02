#include <iostream>
#include <iomanip>
#include <string>
#include <chrono>
#include <thread>

#include "./CLI_View.hpp"


void cli_core::CLI_View::display()
{}

void cli_core::CLI_View::displayTitle(std::string title)
{
    std::cout << std::endl;
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
    std::cin.ignore();

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

void cli_core::CLI_View::progressBar()
{
    float progress = 0.0;

    while (progress <= 1.0) {
        int barWidth = 100;

        std::cout << "[";
        int pos = barWidth * progress;
        for (int i = 0; i < barWidth; ++i) 
        {
            if (i < pos) 
                std::cout << "=";
            else if (i == pos)
                std::cout << ">";
            else 
                std::cout << "-";
        }
        std::cout << "] " << int(progress * 100.0) << " %\r";
        std::cout.flush();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        if (progress == 1)
            break;

        progress += 0.01; // for demonstration only

        if (progress >= 1)
            progress = 1;
    }

    std::cout << std::endl;
}

void cli_core::CLI_View::loadingAnimation(int milliseconds)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
    std::cout << "\b\\" << std::flush;
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
    std::cout << "\b|" << std::flush;
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
    std::cout << "\b/" << std::flush;
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
    std::cout << "\b-" << std::flush;
}