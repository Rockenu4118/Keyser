#include "./TxoSet.hpp"


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

std::vector<keyser::TXO> keyser::TxoSet::selectRingMembers(int numMembers, std::string realMember)
{
    std::vector<keyser::TXO> members;


    return members;
}


bool keyser::TxoSet::isTxoPresent(std::string txoHash) const
{
    if (_txoSet.find(txoHash) == _txoSet.end())
        return false;

    return true;
}

uint64_t keyser::TxoSet::ownerTotalTxo(std::string privViewKey, std::string pubSpendKey) const
{
    uint64_t balance = 0;

    // for (auto utxo : _utxoSet)
    // {
    //     if (utxo.second._output._recipient == owner)
    //         balance += utxo.second._output._amount;
    // }

    return balance;
}

uint64_t keyser::TxoSet::totalTxo() const
{
    uint64_t total = 0;

    for (const auto&[hash, txo] : _txoSet)
    {
        total += txo._output._amount;
    }

    return total;
}

void keyser::TxoSet::printTxoSet() const
{
    if (_txoSet.size() == 0)
    {
        std::cout << "Txo Set empty." << std::endl;
        return;
    }

    for (const auto&[hash, txo] : _txoSet)
    {
        std::cout << txo._output._amount << std::endl;
    }
}

std::unordered_map<std::string, keyser::TXO>& keyser::TxoSet::txoSet()
{
    return _txoSet;
}

