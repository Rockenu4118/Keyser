#include <iostream>
#include <ctime>
#include <vector>
#include <string>

#include <cryptography.hpp>
#include <nlohmann/json.hpp>

#include "./Block.hpp"
#include "./Transaction.hpp"
#include "../utils/utils.hpp"


keyser::Block::Block(nlohmann::json json)
{
    _index           = json["index"];
    _time            = json["time"];
    _nonce           = json["nonce"];
    _prevHash        = json["prevHash"];
    _hash            = json["hash"];

    for (auto& element : json["transactions"])
    {
        Transaction tx(element);
        _transactions.push_back(tx);
    }
}

keyser::Block::Block(uint index, time_t time, std::string prevHash, std::vector<Transaction> transactions)
{
    _index           = index;
    _time            = time;
    _nonce           = 0;
    _prevHash        = prevHash;
    _hash            = "";
    _transactions    = transactions;
}

keyser::BlockHeader keyser::Block::getHeader() const
{
    BlockHeader header;
    header._index    = _index;
    header._time     = _time;
    header._nonce    = _nonce;
    header._prevHash = _prevHash;
    header._hash     = _hash;

    return header;
}

std::string keyser::Block::calcHash()
{
    std::string unhashed = std::to_string(_index) +
                           std::to_string(_time) + 
                           std::to_string(_nonce) + 
                           _prevHash;
                        
    for (auto& tx : _transactions)
        unhashed.append(tx.calcHash());

    std::string hashed = "";

    cryptography::SHA256(unhashed, hashed);

    return hashed;
}

bool keyser::Block::hasValidHash(uint8_t difficulty)
{
    for (uint i = 0 ; i < difficulty ; i++)
    {
        if (_hash[i] != '0') 
        {
            return false;
        }
    }

    return true;
}

bool keyser::Block::hasValidTransactions()
{
    for (int i = 0 ; i < _transactions.size() ; i++)
    {
        if (!_transactions.at(i).isValid())
            return false;
    }
    return true;
}

nlohmann::json keyser::Block::json() const
{
    nlohmann::json json;

    json["index"]         = _index;
    json["time"]          = _time;
    json["nonce"]         = _nonce;
    json["prevHash"]      = _prevHash;
    json["hash"]          = _hash;
    json["transactions"]  = nlohmann::json::array();

    for (auto& tx : _transactions)
    {
        json["transactions"].push_back(tx.json());
    }

    return json;
}

void keyser::Block::json(nlohmann::json json)
{
    _index           = json["index"];
    _time            = json["time"];
    _nonce           = json["nonce"];
    _prevHash        = json["prevHash"];
    _hash            = json["hash"];

    for (auto& element : json["transactions"])
    {
        Transaction tx(element);
        _transactions.push_back(tx);
    }
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
        out << "Index:        " << data._index                  << std::endl;
        out << "Time:         " << utils::localTime(data._time) << std::endl;
        out << "Nonce:        " << data._nonce                  << std::endl;
        out << "Prev Hash:    " << data._prevHash               << std::endl;
        out << "Hash:         " << data._hash                   << std::endl;
        out << "Transactions: " << data._transactions.size()    << std::endl;

        return out;
    }
}