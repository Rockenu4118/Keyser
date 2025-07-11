#ifndef NODE_H
#define NODE_H

#include <memory>
#include <ctime>

#include "../chain/TxoSet.hpp"
#include "../chain/ChainIndex.hpp"
#include "../chain/Miner.hpp"
#include "../chain/Mempool.hpp"
#include "../net/NetInterface.hpp"
#include "../validation/ValidationEngine.hpp"
#include "../wallet/Wallet.hpp"
#include "../primitives/Transaction.hpp"


namespace keyser
{
    // Forward declare
    class TxoSet;
    class ChainIndex;
    class Mempool;
    class NetInterface;
    class MainMenu;

    class Node
    {
    public:
        enum class Status : uint16_t
        {
            Offline,
            Syncing,
            Online
        };

        Status status() const;

        void status(Status status);

        time_t getUptime() const;

        bool submitTransaction(Transaction transaction);

        // std::shared_ptr<TxoSet> _txoSet;
        std::shared_ptr<ChainIndex> mChain;
        // std::shared_ptr<Mempool> _mempool;
        std::shared_ptr<NetInterface> mNetwork;
    private:
        Status _status = Status::Offline;
        time_t _startTime;
    };
}

#endif