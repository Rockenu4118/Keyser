#ifndef PROMPT_H
#define PROMPT_H

#include "../../chain/WalletManager.hpp"
#include "../../node/Node.hpp"

namespace keyser
{
    namespace cli
    {
        void promptNetConfig(int& serverPort, int& clientPort);
        void promptMainMenu(char& selection, bool& miningStatus);
        void promptMiningMenu(Node* node, bool& miningStatus);
        void promptWalletMenu(WalletManager& wallets);
        void promptTransactionMenu(Node* node);
    }
}

#endif