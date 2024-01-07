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

void keyser::utils::encodeJson(std::string& jsonStr, Transaction& transaction)
{
    nlohmann::json doc;

    doc["amount"]            = transaction._amount;
    doc["reciever address"]  = transaction._recieverAddress;
    doc["sender address"]    = transaction._senderAddress;
    doc["sender public key"] = transaction._senderPublicKey;
    doc["hash"]              = transaction._hash;
    doc["r sig val"]         = transaction._rSigVal;
    doc["s sig val"]         = transaction._sSigVal;

    jsonStr = doc.dump();
}

void keyser::utils::decodeJson(Transaction& transaction, std::string& jsonStr)
{
    nlohmann::json json = nlohmann::json::parse(jsonStr);

    transaction._amount          = json["amount"];
    transaction._recieverAddress = json["reciever address"];
    transaction._senderAddress   = json["sender address"];
    transaction._senderPublicKey = json["sender public key"];
    transaction._hash            = json["hash"];
    transaction._rSigVal         = json["r sig val"];
    transaction._sSigVal         = json["s sig val"];
}

void keyser::utils::encodeJson(std::string& jsonStr, Block& block)
{
    nlohmann::json doc;
    nlohmann::json transactionArr;

    doc["index"]     = block._index;
    doc["time"]      = block._time;
    doc["nonce"]     = block._nonce;
    doc["prev hash"] = block._prevHash;
    doc["hash"]      = block._hash;

    for (int i = 0 ; i < block._transactions.size() ; i++)
    {
        std::string txJsonStr;
        encodeJson(txJsonStr, block._transactions.at(i));
        transactionArr.push_back(txJsonStr);
    }

    doc["transactions"] = transactionArr.dump();

    jsonStr = doc.dump();
}

void keyser::utils::decodeJson(Block& block, std::string& jsonStr)
{
    nlohmann::json json = nlohmann::json::parse(jsonStr);

    block._index    = json["index"];
    block._time     = json["time"];
    block._nonce    = json["nonce"];
    block._prevHash = json["prev hash"];
    block._hash     = json["hash"];

    std::string txJsonStr = json["transactions"];
    nlohmann::json txJson = nlohmann::json::parse(txJsonStr);
    
    for (int i = 0 ; i < txJson.size() ; i++)
    {
        std::string txStr = txJson.at(i);
        Transaction tx;
        decodeJson(tx, txStr);
        block._transactions.push_back(tx);
    }
}

void keyser::utils::encodeJson(std::string& jsonStr, NodeInfo& nodeInfo)
{
    nlohmann::json doc;

    doc["version"] = nodeInfo._version;
    doc["alias"]   = nodeInfo._alias;
    doc["address"] = nodeInfo._address;
    doc["port"]    = nodeInfo._port;

    jsonStr = doc.dump();
}

void keyser::utils::decodeJson(NodeInfo& nodeInfo, std::string& jsonStr)
{
    nlohmann::json json = nlohmann::json::parse(jsonStr);

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
