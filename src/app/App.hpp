#ifndef APP_H
#define APP_H

#include "../node/Node.hpp"
#include "../views/MainMenu.hpp"


namespace keyser
{
    static std::atomic mShutdown = false;

    class App
    {
    public:
        App();

        void init(int argc, char* argv[]);

    private:
        Node* mNode;
        MainMenu* mMenu;
        Miner* mMiner;
        Wallet* mWallet;
    };
}

#endif