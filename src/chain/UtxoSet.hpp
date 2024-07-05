#ifndef UTXO_SET_H
#define UTXO_SET_H

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

#include "../node/Node.hpp"
#include "./Transaction.hpp"


namespace keyser
{
    class Node;

    class UtxoSet
    {
        public:
            UtxoSet(Node* node);

            ~UtxoSet() = default;

            void processBlock(std::shared_ptr<Block> block);

            std::vector<UTXO> possibleUtxos(std::string owner) const;

            bool isUtxoPresent(std::string utxoHash) const;

            uint64_t ownerTotalUtxo(std::string owner) const;

            uint64_t totalUtxo() const;

            void printUtxoSet() const;

            std::unordered_map<std::string, UTXO>& utxoSet();

        private:
            Node* _node;

            std::unordered_map<std::string, UTXO> _utxoSet;
    };
}

#endif