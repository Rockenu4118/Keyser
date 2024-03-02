#include <iostream>
#include <ctime>
#include <vector>
#include <string>

#include <cryptography.hpp>

#include "./Block.hpp"
#include "./Transaction.hpp"
#include "../utils/utils.hpp"


keyser::Block::Block(uint index, time_t time, std::string prevHash, double reward, std::string rewardAddress, std::vector<Transaction> transactions)
{
    _index         = index;
    _time          = time;
    _nonce         = 0;
    _prevHash      = prevHash;
    _hash          = "";
    _reward        = reward;
    _rewardAddress = rewardAddress;
    _transactions  = transactions;
}

void keyser::Block::calcHash()
{
    std::string unhashed = std::to_string(_index) +
                           std::to_string(_time) + 
                           std::to_string(_nonce) + 
                           _prevHash + 
                           std::to_string(_reward) +
                           _rewardAddress;
                         
    for (Transaction tx : _transactions)
    {
        unhashed.append(tx._hash);
    }

    std::string hashed = "";

    cryptography::SHA256(unhashed, hashed);

    _hash = hashed;
}

void keyser::Block::calcValidHash(uint8_t difficulty)
{
    bool validHash;

    while (true) 
    {   
        validHash = true;
        _nonce++;
        _time = time(NULL);
        calcHash();

        for (uint i = 0 ; i < difficulty ; i++)
        {
            if (_hash[i] != '0') 
            {
                validHash = false;
                break;
            }
        }

        if (validHash)
            return;
    }
}

// Other
bool keyser::Block::hasValidTransactions()
{
    for (int i = 0 ; i < _transactions.size() ; i++)
    {
        if (!_transactions.at(i).isValid())
            return false;
    }
    return true;
}

void keyser::Block::printTransactions()
{   
    for (uint i = 0 ; i < _transactions.size() ; i++)
    {
        std::cout << "#" << std::to_string(i + 1) << ", ";
        std::cout << _transactions.at(i) << std::endl;
    }
}

namespace keyser
{
    std::ostream& operator<<(std::ostream& out, Block& data) {
        out << "Index:          " << data._index               << std::endl;
        out << "Time:           " << data._time                << std::endl;
        out << "Nonce:          " << data._nonce               << std::endl;
        out << "Prev Hash:      " << data._prevHash            << std::endl;
        out << "Hash:           " << data._hash                << std::endl;
        out << "Reward:         " << data._reward              << std::endl;
        out << "Reward address: " << data._rewardAddress       << std::endl;
        out << "Transactions:   " << data._transactions.size() << std::endl;

        return out;
    }
}