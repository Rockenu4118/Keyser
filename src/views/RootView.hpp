#ifndef ROOT_VIEW_H
#define ROOT_VIEW_H

#include <CLI_View.hpp>

#include "../node/Node.hpp"
#include "../wallet/WalletManager.hpp"


namespace keyser
{
    namespace cli
    {
        class RootView : public cli_core::CLI_View
        {
            public:
                RootView(Node* node, WalletManager& wallets);


            protected:
                void display();


            private:
                Node* _node;
                WalletManager& _wallets;

        };
    }
}

#endif