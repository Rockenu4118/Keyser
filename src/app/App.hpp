#ifndef APP_H
#define APP_H

#include <memory>

#include "../node/Node.hpp"
#include "../views/MainMenu.hpp"

// #include "../rpc/rpc.hpp"


namespace keyser
{
    class MainMenu;

    class App
    {
        public:
            App();

            void run();

        private:
            Node*     mNode;
            MainMenu* mMenu;
            // RPC*  _rpc;
    };
}

#endif