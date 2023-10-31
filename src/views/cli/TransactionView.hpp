#ifndef TRANSACTION_VIEW_H
#define TRANSACTION_VIEW_H

#include "../../chain/WalletManager.hpp"
#include "../../chain/Transaction.hpp"
#include "../../node/Node.hpp"

namespace keyser
{
    namespace cli
    {
        class TransactionView
        {
            public:
                TransactionView(WalletManager& wallets, node::Node* node);

                void display();

            private:
                WalletManager& _wallets;
                node::Node*    _node;
        };
    }
}

#endif