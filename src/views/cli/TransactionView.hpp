#ifndef TRANSACTION_VIEW_H
#define TRANSACTION_VIEW_H

#include <CLI_View.hpp>

#include "../../chain/WalletManager.hpp"
#include "../../chain/Transaction.hpp"
#include "../../node/Node.hpp"

namespace keyser
{
    namespace cli
    {
        class TransactionView : public cli_core::CLI_View
        {
            public:
                TransactionView(WalletManager& wallets, node::Node* node);

            protected:
                void display();
                
            private:
                void newTransaction();

                WalletManager& _wallets;
                node::Node*    _node;
        };
    }
}

#endif