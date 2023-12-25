#ifndef TRANSACTION_VIEW_H
#define TRANSACTION_VIEW_H

#include <CLI_View.hpp>

#include "../../wallet/WalletManager.hpp"
#include "../../chain/Transaction.hpp"
#include "../../node/Node.hpp"

namespace keyser
{
    namespace cli
    {
        class TransactionView : public cli_core::CLI_View
        {
            public:
                TransactionView(WalletManager& wallets, Node* node);

            protected:
                void display();
                
            private:
                void newTransaction();

                WalletManager& _wallets;
                Node*    _node;
        };
    }
}

#endif