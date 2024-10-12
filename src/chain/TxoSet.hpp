#ifndef TXO_SET_H
#define TXO_SET_H

#include <string>
#include <vector>
#include <unordered_map>

#include "../node/Node.hpp"
#include "./Transaction.hpp"
#include "./Block.hpp"


namespace keyser
{
    class Node;

    class TxoSet
    {
        public:
            explicit TxoSet(Node* node);

            ~TxoSet() = default;

            void processBlock(Block& block);

            std::vector<TXO> selectRingMembers(int numMembers, std::string realMember);

            bool isTxoPresent(std::string txoHash) const;

            uint64_t ownerTotalTxo(std::string privViewKey, std::string pubSpendKey) const;

            uint64_t totalTxo() const;

            void printTxoSet() const;

            std::unordered_map<std::string, TXO>& txoSet();

        private:
            Node* _node;

            std::unordered_map<std::string, TXO> _txoSet;
    };
}

#endif