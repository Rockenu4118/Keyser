#include <iostream>
#include <ctime>
#include <vector>
#include <string>

#include <cryptography.hpp>

#include "./include/Block.hpp"
#include "./include/Transaction.hpp"



// Constructors
Keyser::Block::Block(uint index, Block* prevBlock, std::string prevHash, std::vector<Transaction> transactions)
{
    _prevBlock    = prevBlock;
    _index        = index;
    _time         = time(NULL);
    _nonce        = 0;
    _prevHash     = prevHash;
    _hash         = "";
    _transactions = transactions;
}

// Accessors
Keyser::Block* Keyser::Block::getPrevBlock()
{
    return _prevBlock;
}

uint Keyser::Block::getIndex()
{
    return _index;
}

time_t Keyser::Block::getTime()
{
    return _time;
}

uint32_t Keyser::Block::getNonce()
{
    return _nonce;
}

std::string Keyser::Block::getPrevHash()
{
    return _prevHash;
}

std::string Keyser::Block::getHash()
{
    return _hash;
}

std::vector<Keyser::Transaction> Keyser::Block::getTransactions()
{
    return _transactions;
}

// Modifiers
void Keyser::Block::calcHash()
{
    std::string unhashed = _prevHash + std::to_string(_time) + std::to_string(_nonce);
    std::string hashed = "";

    cryptography::sha256(unhashed, hashed);

    _hash = hashed;
}

void Keyser::Block::calcValidHash(uint8_t difficulty)
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
void Keyser::Block::printTransactions()
{
    std::cout << std::endl;
    std::cout << "Transactions, Block #" << _index << "" << std::endl;
    
    for (uint i = 0 ; i < _transactions.size() ; i++)
    {
        std::cout << "Transaction # " << std::to_string(i + 1) << std::endl;

        std::cout << "Amount: "     << _transactions.at(i).getAmount();
        std::cout << ", Reciever: " << _transactions.at(i).getRecieverAddress();
        std::cout << ", Sender: "   << _transactions.at(i).getSenderAddress();
    
        std::cout << std::endl;
    }
}

namespace Keyser
{
    std::ostream& operator<<(std::ostream& out, Block& data) {
        out << "Index:        " << data.getIndex()               << std::endl;
        out << "Time:         " << data.getTime()                << std::endl;
        out << "Nonce:        " << data.getNonce()               << std::endl;
        out << "Prev Hash:    " << data.getPrevHash()            << std::endl;
        out << "Hash:         " << data.getHash()                << std::endl;
        out << "Transactions: " << data.getTransactions().size() << std::endl;

        return out;
    }
}