#include "MainMenu.hpp"

#include "../app/App.hpp"


keyser::MainMenu::MainMenu(Node* node) : mNode(node)
{}

void keyser::MainMenu::start()
{
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, true);
    nodelay(stdscr, true);

    int termHeight, termWidth;
    getmaxyx(stdscr, termHeight, termWidth);
    const int logHeight = termHeight - TITLE_HEIGHT - INPUT_HEIGHT;

    mTitleWindow = newwin(TITLE_HEIGHT, termWidth, 0, 0);
    mLogWindow = newwin(logHeight, termWidth, TITLE_HEIGHT, 0);
    mInputWindow = newwin(INPUT_HEIGHT, termWidth, TITLE_HEIGHT + logHeight, 0);

    while (!mShutdown)
    {
        int ch = getch();

        // Quit
        if (ch == 27) break;
        if (ch == '\n' || ch == KEY_ENTER)
        {
            handleInput(mText);
            mText.clear();
        } else if (ch == KEY_BACKSPACE)
        {
            if (!mText.empty())
                mText.pop_back();
        } else if (isprint(ch))
        {
            mText.push_back(static_cast<char>(ch));
        }

        werase(mTitleWindow);
        wattron(mTitleWindow, A_REVERSE);
        mvwprintw(mTitleWindow, 0, (termWidth - 30) / 2, " Keyser Protocol - v1.0 ");
        wattroff(mTitleWindow, A_REVERSE);
        wrefresh(mTitleWindow);

        // ===== MAIN CONTENT =====
        werase(mLogWindow);
        box(mLogWindow, 0, 0);

        switch (mMode)
        {
        case LOG:
            {
                mvwprintw(mLogWindow, 0, 2, "%s", "Log");
                int start = mLog.size() > logHeight - 2 ? mLog.size() - (logHeight - 2) : 0;
                for (int i = start ; i < mLog.size() ; ++i) {
                    mvwprintw(mLogWindow, i - start + 1, 1, "%s %s", mLog.at(i).time.c_str(), mLog.at(i).text.c_str());
                }
            }
            break;
        case CONNECTIONS:
            {
                mvwprintw(mLogWindow, 0, 2, "%s", "Connections");
                for (int i = 0 ; i < mNode->mNetwork->mPeers.size() ; i++)
                {
                    mvwprintw(mLogWindow, i + 1, 1, "%s", mNode->mNetwork->mPeers.at(i)->toString().c_str());
                }
            }
            break;
        case BLOCKS:
            {
                mvwprintw(mLogWindow, 0, 2, "%s", "Blocks");
                // int start = mLog.size() > logHeight - 2 ? mLog.size() - (logHeight - 2) : 0;
                // for (int i = 0)
            }
            break;
        }

        wrefresh(mLogWindow);
        // ========================

        werase(mInputWindow);
        box(mInputWindow, 0, 0);
        mvwprintw(mInputWindow, 1, 2, "> %s", mText.c_str());
        wmove(mInputWindow, 1, 4 + mText.size());
        wrefresh(mInputWindow);

        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }

    delwin(mTitleWindow);
    delwin(mLogWindow);
    delwin(mInputWindow);
    endwin();
}

void keyser::MainMenu::log(const std::string& text)
{
    mLog.push_back({utils::localTimestamp(), text});
}

void keyser::MainMenu::handleInput(const std::string& text)
{
    if (text.empty()) return;
    std::vector<std::string> args = utils::parse_cmd(text);
    std::string cmd = args.at(0);

    if (cmd == "/help") {}
    else if (cmd == "/log")         { mMode = LOG; }
    else if (cmd == "/connections") { mMode = CONNECTIONS; }
    else if (cmd == "/ping")        { mNode->mNetwork->ping(); }
    else if (cmd == "/startserver") { mNode->mNetwork->start(); }
    else if (cmd == "/shutdown")    { mShutdown = true;  }
    else if (cmd.empty())           { log("Invalid command"); }
    else { log(cmd); }
}


