#ifndef HOME_VIEW_H
#define HOME_VIEW_H

#include <CLI_View.hpp>

#include "../../chain/WalletManager.hpp"
#include "../../node/Node.hpp"

namespace keyser
{
    namespace cli
    {
        class HomeView : public cli_core::CLI_View
        {
            public:
                HomeView();

            protected:
                void display();

            private:
                void promptNetConfig();
                void promptNodeType();

                node::Node*    _node;
                WalletManager& _wallets;
                bool&          _miningStatus;
        };
    }
}

#endif