#ifndef PROMPT_H
#define PROMPT_H

#include "../../../chain/include/WalletCollection.hpp"

namespace Keyser
{
    namespace cli
    {
        void promptNetConfig(int& serverPort, int& clientPort);
        void promptMainMenu(int& selection, bool& miningStatus);
        void promptWalletMenu(WalletCollection& wallets);
        void promptTransactionMenu();
    }
}

#endif