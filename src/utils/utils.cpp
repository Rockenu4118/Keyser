#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <ctime>
#include <algorithm>
#include <cryptography.hpp>
#include <nlohmann/json.hpp>

#include "./utils.hpp"
#include "../wallet/Wallet.hpp"
#include "../chain/Transaction.hpp"
#include "../chain/Block.hpp"
#include "../node/NodeInfo.hpp"


std::string keyser::utils::pubKeytoAddress(const std::string& uPublicKey)
{   
    // Step 1: Convert the raw hex characters of the uncompressed public key to a string.
    std::string unhashed = hexToString(uPublicKey);
    std::string hashed = "";

    // Step 2: Generate a sha256 hash of the uncompressed public key.
    cryptography::SHA256(unhashed, hashed);

    // Step 3: Extract last 32 (hex) characters, 16 bytes, of the sha256 hash.
    std::string address = hashed.substr(32, 32);

    // Step 4: Prepend "0x" to indicate that the address is in hex format.
    std::string publicAddress = "0x" + address;

    return publicAddress;
}

std::string keyser::utils::hexToString(const std::string& input)
{
    std::string output;
    if ((input.length() % 2) != 0) {
        std::cout << "String is not valid length." << std::endl;
    }
    size_t count = input.length() / 2;
    for (size_t i = 0 ; count > i ; i++) {
        uint32_t s = 0;
        std::stringstream ss;
        ss << std::hex << input.substr(i * 2, 2);
        ss >> s;
        output.push_back(static_cast<unsigned char>(s));
    }
    return output;
}

void keyser::utils::encodeJson(nlohmann::json& json, Transaction& transaction)
{
    json["time"]            = transaction._time;
    json["amount"]          = transaction._amount;
    json["recieverAddress"] = transaction._recieverAddress;
    json["senderAddress"]   = transaction._senderAddress;
    json["senderPublicKey"] = transaction._senderPublicKey;
    json["hash"]            = transaction._hash;
    json["rSigVal"]         = transaction._rSigVal;
    json["sSigVal"]         = transaction._sSigVal;
}

void keyser::utils::decodeJson(Transaction& transaction, nlohmann::json& json)
{
    transaction._time            = json["time"];
    transaction._amount          = json["amount"];
    transaction._recieverAddress = json["recieverAddress"];
    transaction._senderAddress   = json["senderAddress"];
    transaction._senderPublicKey = json["senderPublicKey"];
    transaction._hash            = json["hash"];
    transaction._rSigVal         = json["rSigVal"];
    transaction._sSigVal         = json["sSigVal"];
}

// void keyser::utils::encodeJson(nlohmann::json& json, BlockHeader& header)
// {
//     json["index"]         = header._index;
//     json["time"]          = header._time;
//     json["nonce"]         = header._nonce;
//     json["prev hash"]     = header._prevHash;
//     json["hash"]          = header._hash;
//     json["reward"]        = header._reward;
//     json["rewardAddress"] = header._rewardAddress;
// }

// void keyser::utils::decodeJson(BlockHeader& header, nlohmann::json& json)
// {
//     header._index         = json["index"];
//     header._time          = json["time"];
//     header._nonce         = json["nonce"];
//     header._prevHash      = json["prev hash"];
//     header._hash          = json["hash"];
//     header._reward        = json["reward"];
//     header._rewardAddress = json["rewardAddress"];
// }

void keyser::utils::encodeJson(nlohmann::json& json, Block& block)
{
    json["index"]         = block._index;
    json["time"]          = block._time;
    json["nonce"]         = block._nonce;
    json["prevHash"]      = block._prevHash;
    json["hash"]          = block._hash;
    json["reward"]        = block._reward;
    json["rewardAddress"] = block._rewardAddress;
    json["transactions"]  = nlohmann::json::array();

    for (Transaction tx : block._transactions)
    {
        nlohmann::json txJson;
        encodeJson(txJson, tx);
        json["transactions"].push_back(txJson);
    }
}

void keyser::utils::decodeJson(Block& block, nlohmann::json& json)
{
    block._index         = json["index"];
    block._time          = json["time"];
    block._nonce         = json["nonce"];
    block._prevHash      = json["prevHash"];
    block._hash          = json["hash"];
    block._reward        = json["reward"];
    block._rewardAddress = json["rewardAddress"];

    for (auto& element : json["transactions"])
    {
        Transaction tx;
        decodeJson(tx, element);
        block._transactions.push_back(tx);
    }
}

void keyser::utils::encodeJson(nlohmann::json& json, NodeInfo& nodeInfo)
{
    json["version"] = nodeInfo._version;
    json["alias"]   = nodeInfo._alias;
    json["address"] = nodeInfo._address;
    json["port"]    = nodeInfo._port;
}

void keyser::utils::decodeJson(NodeInfo& nodeInfo, nlohmann::json& json)
{
    nodeInfo._version = json["version"];
    nodeInfo._alias   = json["alias"];
    nodeInfo._address = json["address"];
    nodeInfo._port    = json["port"];
}

std::string keyser::utils::localTimestamp()
{
    time_t t = time(NULL);
    struct tm *tmp = localtime(&t);

    std::string hourStr = std::to_string(tmp->tm_hour);
    std::string minStr  = std::to_string(tmp->tm_min);
    std::string secStr  = std::to_string(tmp->tm_sec);

    int hourStrLen = hourStr.size();
    int minStrLen  = minStr.size();
    int secStrLen  = secStr.size();

    std::string timestamp = "[" + 
                            std::string(2 - std::min(2, hourStrLen), '0') + hourStr + ":" +
                            std::string(2 - std::min(2, minStrLen), '0')  + minStr  + ":" +
                            std::string(2 - std::min(2, secStrLen), '0')  + secStr  + "] ";

    return timestamp;
}

std::string keyser::utils::localTime(time_t time)
{
    struct tm *tmp = localtime(&time);

    std::string hourStr = std::to_string(tmp->tm_hour);
    std::string minStr  = std::to_string(tmp->tm_min);
    std::string secStr  = std::to_string(tmp->tm_sec);

    int hourStrLen = hourStr.size();
    int minStrLen  = minStr.size();
    int secStrLen  = secStr.size();

    std::string localTime = std::string(2 - std::min(2, hourStrLen), '0') + hourStr + ":" +
                            std::string(2 - std::min(2, minStrLen), '0')  + minStr  + ":" +
                            std::string(2 - std::min(2, secStrLen), '0')  + secStr;

    return localTime;
}
