#ifndef APP_H
#define APP_H

#include <memory>

#include "../views/RootView.hpp"
#include "../node/Node.hpp"
#include "../rpc/rpc.hpp"


namespace keyser
{
    class App
    {
        public:
            App();

            void run();

        private:
            Node* _node;
            RPC*  _rpc;

            cli::RootView* _view;

    };
}

#endif