#include <iostream>
#include <ctime>
#include <vector>
#include <string>

#include <cryptography.hpp>
#include <nlohmann/json.hpp>

#include "./Block.hpp"
#include "./Transaction.hpp"
#include "../utils/utils.hpp"
#include "../data/version.hpp"


keyser::BlockHeader::BlockHeader(nlohmann::json json)
{
    _index           = json["index"];
    _version         = json["version"];
    _prevHash        = json["prevHash"];
    _bodyHash        = json["bodyHash"];
    _time            = json["time"];
    _nonce           = json["nonce"];
}

nlohmann::json keyser::BlockHeader::json() const
{
    nlohmann::json json;

    json["index"]    = _index;
    json["version"]  = _version;
    json["prevHash"] = _prevHash;
    json["bodyHash"] = _bodyHash;
    json["time"]     = _time;
    json["nonce"]    = _nonce;

    return json;
}

std::string keyser::BlockHeader::hash() const
{
    std::string unhashed = std::to_string(_index) + 
                           _version + 
                           _prevHash + 
                           _bodyHash + 
                           _reward.hash();
                           std::to_string(_time) +
                           std::to_string(_nonce);
    std::string hashed;

    cryptography::SHA256(unhashed, hashed);

    return hashed;
}

keyser::Block::Block(nlohmann::json json)
{
    _index    = json["index"];
    _version  = json["version"];
    _prevHash = json["prevHash"];
    _bodyHash = json["bodyHash"];
    _reward     = Transaction(json["reward"]);
    _time     = json["time"];
    _nonce    = json["nonce"];

    for (auto& element : json["transactions"])
    {
        Transaction tx(element);
        _transactions.push_back(tx);
    }
}

keyser::Block::Block(uint index, time_t time, std::string prevHash, Transaction reward, std::vector<Transaction> transactions)
{
    _index           = index;
    _version         = KEYSER_VERSION;
    _prevHash        = prevHash;
    _reward          = reward;
    _time            = time;
    _nonce           = 0;
    _transactions    = transactions;
}

keyser::BlockHeader keyser::Block::getHeader() const
{
    BlockHeader header;
    header._index    = _index;
    header._version  = _version;
    header._prevHash = _prevHash;
    header._bodyHash = _bodyHash;
    header._reward   = _reward;
    header._time     = _time;
    header._nonce    = _nonce;
    
    return header;
}

std::string keyser::Block::bodyHash() const
{
    std::string unhashed;
    std::string hashed;
                        
    for (auto& tx : _transactions)
        unhashed.append(tx.hash());

    cryptography::SHA256(unhashed, hashed);

    return hashed;
}

bool keyser::Block::hasValidTransactions()
{
    for (const auto & _transaction : _transactions)
    {
        if (!_transaction.isSigned())
            return false;
    }
    return true;
}

nlohmann::json keyser::Block::json() const
{
    nlohmann::json json;

    json["index"]         = _index;
    json["version"]       = _version;
    json["prevHash"]      = _prevHash;
    json["bodyHash"]      = _bodyHash;
    json["reward"]        = _reward.json();
    json["time"]          = _time;
    json["nonce"]         = _nonce;
    json["transactions"]  = nlohmann::json::array();

    for (auto& tx : _transactions)
    {
        json["transactions"].push_back(tx.json());
    }

    return json;
}

void keyser::Block::printTransactions()
{   
    for (auto& tx : _transactions)
    {
        std::cout << tx << std::endl;
    }
}

namespace keyser
{
    std::ostream& operator<<(std::ostream& out, const BlockHeader& data) {
        out << "Index:        " << data._index                  << std::endl;
        out << "Prev Hash:    " << data._prevHash               << std::endl;
        out << "Body Hash:    " << data._bodyHash               << std::endl;
        out << "Hash:         " << data.hash()                  << std::endl;
        out << "Time:         " << utils::localTime(data._time) << std::endl;
        out << "Nonce:        " << data._nonce                  << std::endl;

        return out;
    }

    std::ostream& operator<<(std::ostream& out, const Block& data) {
        out << "Index:        " << data._index                  << std::endl;
        out << "Prev Hash:    " << data._prevHash               << std::endl;
        out << "Body Hash:    " << data._bodyHash               << std::endl;
        out << "Hash:         " << data.hash()                  << std::endl;
        out << "Time:         " << utils::localTime(data._time) << std::endl;
        out << "Nonce:        " << data._nonce                  << std::endl;
        out << "Transactions: " << data._transactions.size()    << std::endl;

        return out;
    }
}