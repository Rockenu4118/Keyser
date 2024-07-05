#ifndef NODE_H
#define NODE_H

#include <memory>

#include "../net/Peer.hpp"
#include "../chain/UtxoSet.hpp"
#include "../chain/Chain.hpp"
#include "../chain/Miner.hpp"
#include "../chain/Mempool.hpp"
#include "../net/NetInterface.hpp"
#include "../validation/ValidationEngine.hpp"
#include "../wallet/Wallet.hpp"
#include "../chain/Transaction.hpp"

namespace keyser
{
    // Forward declare
    class UtxoSet;
    class Chain;
    class Miner;
    class Mempool;
    class NetInterface;
    class ValidationEngine;
    class Wallet;

    class Node
    {
    public:
        enum class Status : uint16_t
        {
            Offline,
            Syncing,
            Online
        };

        Node(uint16_t port);

        ~Node();

        void run();

        void shutdown();

        Status status() const;

        void status(Status status);

        time_t getUptime() const;

        bool shutdownFlag() const;

        void beginMining(uint numBlocks = -1);

        void stopMining();

        bool submitTransaction(Transaction transaction);

        std::shared_ptr<Chain>& chain();

        std::shared_ptr<Miner>& miner();

        std::shared_ptr<Mempool>& mempool();

        std::shared_ptr<UtxoSet>& utxoSet();

        std::shared_ptr<NetInterface>& network();

        std::shared_ptr<ValidationEngine>& validationEngine();

        std::shared_ptr<Wallet>& wallet();

    private:
        Status _status = Status::Offline;
        time_t _startTime;

        std::shared_ptr<UtxoSet> _utxoSet;
        std::shared_ptr<Chain> _chain;
        std::shared_ptr<Miner> _miner;
        std::shared_ptr<Mempool> _mempool;
        std::shared_ptr<NetInterface> _network;
        std::shared_ptr<ValidationEngine> _validationEngine;
        std::shared_ptr<Wallet> _wallet;

        bool _shutdownNode = false;
    };
}

#endif