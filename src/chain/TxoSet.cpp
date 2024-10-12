#include <iostream>

#include "./UtxoSet.hpp"


keyser::TxoSet::TxoSet(Node* node) : _node(node)
{}

void keyser::TxoSet::processBlock(Block& block)
{
    // auto iter = _utxoSet

    // for (auto tx : block._transactions)
    // {
    //     for (auto input : tx._inputs)
    //     {
    //         auto iter = _utxoSet.find(input._outPoint.calcHash());
    //         if (iter != _utxoSet.end())
    //             _utxoSet.erase(iter);

    //     }

    //     for (int i = 0 ; i < tx._outputs.size() ; i++)
    //     {
    //         UTXO utxo;
    //         utxo._outPoint._txHash = tx.hash();
    //         utxo._outPoint._index = i;
    //         utxo._output = tx._outputs.at(i);

    //         _utxoSet[utxo._outPoint.calcHash()] = utxo;
    //     }
    // }
}

std::vector<keyser::TXO> keyser::TxoSet::possibleUtxos(std::string owner) const
{
    std::vector<TXO> utxos;

    for (auto utxo : _utxoSet)
    {
        if (utxo.second._output._recipient == owner)
            utxos.push_back(utxo.second);
    }

    return utxos;
}

bool keyser::TxoSet::isUtxoPresent(std::string utxoHash) const
{
    if (_utxoSet.find(utxoHash) == _utxoSet.end())
        return false;

    return true;
}

uint64_t keyser::TxoSet::ownerTotalUtxo(std::string owner) const
{
    uint64_t balance = 0;

    for (auto utxo : _utxoSet)
    {
        if (utxo.second._output._recipient == owner)
            balance += utxo.second._output._amount;
    }

    return balance;
}

uint64_t keyser::TxoSet::totalUtxo() const
{
    uint64_t total = 0;

    for (auto utxo : _utxoSet)
    {
        total += utxo.second._output._amount;
    }

    return total;
}

void keyser::TxoSet::printUtxoSet() const
{
    // if (_utxoSet.size() == 0)
    // {
    //     std::cout << "Utxo Set empty." << std::endl;
    //     return;
    // }
    //
    // for (auto utxo : _utxoSet)
    // {
    //     std::cout << utxo.second << std::endl;
    // }
}

std::unordered_map<std::string, keyser::TXO>& keyser::TxoSet::utxoSet()
{
    return _utxoSet;
}

