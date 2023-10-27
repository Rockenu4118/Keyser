#include <iostream>
#include <ctime>
#include <vector>
#include <string>

#include <cryptography.hpp>

#include "./Block.hpp"
#include "./Transaction.hpp"



// Constructors
keyser::Block::Block(uint index, Block* prevBlock, std::string prevHash, std::vector<Transaction> transactions)
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
keyser::Block* keyser::Block::getPrevBlock()
{
    return _prevBlock;
}

uint keyser::Block::getIndex()
{
    return _index;
}

time_t keyser::Block::getTime()
{
    return _time;
}

uint32_t keyser::Block::getNonce()
{
    return _nonce;
}

std::string keyser::Block::getPrevHash()
{
    return _prevHash;
}

std::string keyser::Block::getHash()
{
    return _hash;
}

std::vector<keyser::Transaction> keyser::Block::getTransactions()
{
    return _transactions;
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

        std::cout << "Amount: "     << _transactions.at(i).getAmount();
        std::cout << ", Reciever: " << _transactions.at(i).getRecieverAddress();
        std::cout << ", Sender: "   << _transactions.at(i).getSenderAddress();
    
        std::cout << std::endl;
    }
}

namespace keyser
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