#ifndef TRANSACTION_VIEW_H
#define TRANSACTION_VIEW_H

#include "../../chain/WalletManager.hpp"
#include "../../chain/Transaction.hpp"

namespace keyser
{
    namespace cli
    {
        class TransactionView
        {
            public:
                TransactionView(WalletManager& wallets);

                void display();

            private:
                WalletManager& _wallets;
        };
    }
}

#endif