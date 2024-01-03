#ifndef WALLET_VIEW_H
#define WALLET_VIEW_H

#include <CLI_View.hpp>

#include "../wallet/WalletManager.hpp"


namespace keyser
{
    namespace cli
    {
        class WalletView : public cli_core::CLI_View
        {
            public:
                WalletView(WalletManager& wallets);

                void viewWallets();

            protected:
                void display();

            private:
                WalletManager& _wallets;
                
        };
    }
}

#endif