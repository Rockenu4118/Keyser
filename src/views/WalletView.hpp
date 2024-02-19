#ifndef WALLET_VIEW_H
#define WALLET_VIEW_H

#include <CLI_View.hpp>

#include "../node/Node.hpp"


namespace keyser
{
    namespace cli
    {
        class WalletView : public cli_core::CLI_View
        {
            public:
                WalletView(Node* node);

                void viewWallets();
                void viewBalance();
                void createWallet();

            protected:
                void display();

            private:
                Node* _node = nullptr;
                
        };
    }
}

#endif