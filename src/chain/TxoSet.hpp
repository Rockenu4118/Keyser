#ifndef UTXO_SET_H
#define UTXO_SET_H

#include <string>
#include <vector>
#include <unordered_map>

#include "../node/Node.hpp"
#include "./Transaction.hpp"


namespace keyser
{
    class Node;

    class TxoSet
    {
        public:
            explicit TxoSet(Node* node);

            ~TxoSet() = default;

            void processBlock(Block& block);

            std::vector<TXO> possibleUtxos(std::string owner) const;

            bool isUtxoPresent(std::string utxoHash) const;

            uint64_t ownerTotalUtxo(std::string owner) const;

            uint64_t totalUtxo() const;

            void printUtxoSet() const;

            std::unordered_map<std::string, TXO>& utxoSet();

        private:
            Node* _node;

            std::unordered_map<std::string, TXO> _utxoSet;
    };
}

#endif