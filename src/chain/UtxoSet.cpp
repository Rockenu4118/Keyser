#include <iostream>

#include "./UtxoSet.hpp"


keyser::UtxoSet::UtxoSet(Node* node) : _node(node)
{}

void keyser::UtxoSet::processBlock(std::shared_ptr<Block> block)
{
    for (auto tx : block->_transactions)
    {
        for (auto input : tx._inputs)
        {
            auto iter = _utxoSet.find(input._outPoint.calcHash());
            if (iter != _utxoSet.end())
                _utxoSet.erase(iter);

        }

        for (int i = 0 ; i < tx._outputs.size() ; i++)
        {
            UTXO utxo;
            utxo._outPoint._txHash = tx.hash();
            utxo._outPoint._index = i;
            utxo._output = tx._outputs.at(i);

            _utxoSet[utxo._outPoint.calcHash()] = utxo;
        }
    }
}

std::vector<keyser::UTXO> keyser::UtxoSet::possibleUtxos(std::string owner) const
{
    std::vector<UTXO> utxos;

    for (auto utxo : _utxoSet)
    {
        if (utxo.second._output._recipient == owner)
            utxos.push_back(utxo.second);
    }

    return utxos;
}

bool keyser::UtxoSet::isUtxoPresent(std::string utxoHash) const
{
    if (_utxoSet.find(utxoHash) == _utxoSet.end())
        return false;

    return true;
}

uint64_t keyser::UtxoSet::ownerTotalUtxo(std::string owner) const
{
    uint64_t balance = 0;

    for (auto utxo : _utxoSet)
    {
        if (utxo.second._output._recipient == owner)
            balance += utxo.second._output._amount;
    }

    return balance;
}

uint64_t keyser::UtxoSet::totalUtxo() const
{
    uint64_t total = 0;

    for (auto utxo : _utxoSet)
    {
        total += utxo.second._output._amount;
    }

    return total;
}

void keyser::UtxoSet::printUtxoSet() const
{
    if (_utxoSet.size() == 0)
    {
        std::cout << "Utxo Set empty." << std::endl;
        return;
    }
    
    for (auto utxo : _utxoSet)
    {
        std::cout << utxo.second << std::endl;
    }
}

std::unordered_map<std::string, keyser::UTXO>& keyser::UtxoSet::utxoSet()
{
    return _utxoSet;
}

