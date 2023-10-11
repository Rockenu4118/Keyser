#ifndef PROMPT_H
#define PROMPT_H

#include "./WalletCollection.hpp"

namespace Keyser
{
    namespace cli
    {
        void promptNetConfig(int& serverPort, int& clientPort);
        void promptMainMenu(int& selection);
        void promptWallets(WalletCollection& wallets);
        void promptTransactionMenu();
    }
}

#endif