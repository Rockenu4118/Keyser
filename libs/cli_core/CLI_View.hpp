#ifndef CLI_VIEW_H
#define CLI_VIEW_H

#include <string>

namespace cli_core
{
    class CLI_View
    {
        public:
            CLI_View() = default;

        protected:
            void display();
            void displayTitle(std::string title);
            void promptSelection(char& selection);
            void promptConfirmation(char& confirmation);

            void promptInput(std::string msg, std::string& input, bool extraEndl = false);
            void promptInput(std::string msg, double& input,      bool extraEndl = false);
            void promptInput(std::string msg, uint& input,        bool extraEndl = false);

            void clearScreen();
            void continueMsg();
    };
}

#endif