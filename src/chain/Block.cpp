#include <iostream>
#include <ctime>
#include <vector>
#include <string>

#include <nlohmann/json.hpp>

#include "./Block.hpp"
#include "./Transaction.hpp"
#include "../utils/utils.hpp"
#include "../data/version.hpp"


// keyser::BlockHeader::BlockHeader(nlohmann::json json)
// {
//     _index           = json["index"];
//     _version         = json["version"];
//     _prevHash        = json["prevHash"];
//     _bodyHash        = json["bodyHash"];
//     _time            = json["time"];
//     _nonce           = json["nonce"];
// }

// nlohmann::json keyser::BlockHeader::json() const
// {
//     nlohmann::json json;
//
//     json["index"]    = _index;
//     json["version"]  = _version;
//     json["prevHash"] = _prevHash;
//     json["bodyHash"] = _bodyHash;
//     json["time"]     = _time;
//     json["nonce"]    = _nonce;
//
//     return json;
// }

// std::string keyser::BlockHeader::hash() const
// {
    // std::string unhashed = std::to_string(_index) +
    //                        _version +
    //                        _prevHash +
    //                        _bodyHash +
    //                        _reward.hash();
    //                        std::to_string(_time) +
    //                        std::to_string(_nonce);
    // std::string hashed;

    // crypto::hash::SHA256(unhashed, hashed);

    // return hashed;
// }

// keyser::Block::Block(nlohmann::json json)
// {
//     _index    = json["index"];
//     _version  = json["version"];
//     _prevHash = json["prevHash"];
//     _bodyHash = json["bodyHash"];
//     _reward     = Transaction(json["reward"]);
//     _time     = json["time"];
//     _nonce    = json["nonce"];
//
//     for (auto& element : json["transactions"])
//     {
//         Transaction tx(element);
//         _transactions.push_back(tx);
//     }
// }

void keyser::hashHeader(BlockHeader& header, char hash[])
{

}


keyser::Block::Block(uint index, time_t time, unsigned char prevHash[], Transaction reward, std::vector<Transaction> transactions)
{
    mIndex           = index;
    mVersion         = KEYSER_VERSION;
    memcpy(mPrevHash, prevHash, SHA256_DIGEST_LENGTH);
    // _prevHash        = prevHash;
    mReward          = reward;
    mTime            = time;
    mNonce           = 0;
    mTransactions    = transactions;
}

keyser::BlockHeader keyser::Block::getHeader() const
{
    BlockHeader header;
    // header._index    = _index;
    // header._version  = _version;
    // header._prevHash = _prevHash;
    // header._bodyHash = _bodyHash;
    // header._reward   = _reward;
    // header._time     = _time;
    // header._nonce    = _nonce;
    
    return header;
}

void keyser::calcMerkleRoot(std::vector<Transaction>& transactions, unsigned char merkleRoot[])
{
    unsigned char* buf;
    size_t offset = 0;

    for (int i = 0 ; i < transactions.size() ; i++)
    {
        unsigned char hash[SHA256_DIGEST_LENGTH];
        hashTransaction(transactions[i], hash);
    }

    if (!SHA256(buf, offset, merkleRoot))
    {
        throw std::runtime_error("Failed to calc merkleRoot");
    }
}
