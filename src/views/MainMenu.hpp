#ifndef MAINMENU_H
#define MAINMENU_H

#pragma push_macro("timeout")
#pragma push_macro("stdscr")
#undef timeout
#undef stdscr
#include <ncurses.h>
#pragma pop_macro("stdscr")
#pragma pop_macro("timeout")
#include <thread>
#include <vector>

#include "../utils/utils.hpp"


namespace keyser
{
    enum Mode
    {
        LOG,
        CONNECTIONS,
        BLOCKS
    };

    struct LogMessage
    {
        std::string time;
        std::string text;
    };

    class MainMenu {
    public:
        MainMenu(Node* node);

        void start();

        void log(const std::string& text);

        void handleInput(const std::string& text);
    private:
        Node* mNode = nullptr;

        Mode mMode = LOG;

        const int TITLE_HEIGHT = 1;
        const int INPUT_HEIGHT = 3;

        WINDOW *mTitleWindow;
        WINDOW* mLogWindow;
        WINDOW* mInputWindow;

        std::string mText;

        std::vector<LogMessage> mLog;
    };
}



#endif //MAINMENU_H
