#include <iostream>
#include <ctime>
#include <vector>
#include <string>

#include <cryptography.hpp>

#include "./Block.hpp"
#include "./Transaction.hpp"



// Constructors
keyser::Block::Block(uint index, time_t time, std::string prevHash, std::vector<Transaction> transactions)
{
    _index        = index;
    _time         = time;
    _nonce        = 0;
    _prevHash     = prevHash;
    _hash         = "";
    _transactions = transactions;
}

// Modifiers
void keyser::Block::calcHash()
{
    std::string unhashed = _prevHash + std::to_string(_time) + std::to_string(_nonce);
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
void keyser::Block::printTransactions()
{
    std::cout << std::endl;
    std::cout << "Transactions, Block #" << _index << "" << std::endl;
    
    for (uint i = 0 ; i < _transactions.size() ; i++)
    {
        std::cout << "Transaction # " << std::to_string(i + 1) << std::endl;

        std::cout << "Amount: "     << _transactions.at(i)._amount;
        std::cout << ", Reciever: " << _transactions.at(i)._recieverAddress;
        std::cout << ", Sender: "   << _transactions.at(i)._senderAddress;
    
        std::cout << std::endl;
    }
}

namespace keyser
{
    std::ostream& operator<<(std::ostream& out, Block& data) {
        out << "Index:        " << data._index               << std::endl;
        out << "Time:         " << data._time                << std::endl;
        out << "Nonce:        " << data._nonce               << std::endl;
        out << "Prev Hash:    " << data._prevHash            << std::endl;
        out << "Hash:         " << data._hash                << std::endl;
        out << "Transactions: " << data._transactions.size() << std::endl;

        return out;
    }
}