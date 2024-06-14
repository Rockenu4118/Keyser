#include <iostream>
#include <string>

#include <cryptography.hpp>
#include <nlohmann/json.hpp>

#include "../data/version.hpp"
#include "./Transaction.hpp"
#include "../wallet/Wallet.hpp"
#include "../utils/utils.hpp"



// Constructors
keyser::Transaction::Transaction(nlohmann::json json)
{
    _version = json["version"];
    _time    = json["time"];

    for (auto& input : json["inputs"])
    {
        TxIn txin(input);
        _inputs.push_back(txin);
    }

    for (auto& output : json["outputs"])
    {
        TxOut txout(output);
        _outputs.push_back(txout);
    }

    _sig = TxSig(json["signature"]);
}

keyser::Transaction::Transaction(double amount, std::string recievingAddress, std::string sendingPubKey)
{
    _version = KEYSER_VERSION;
    _time    = time(NULL);


    // _amount          = amount;
    // _recieverAddress = recievingAddress;
    // _senderAddress   = keyser::utils::pubKeytoAddress(sendingPubKey);
    // _senderPublicKey = sendingPubKey;
}

// Modifiers
std::string keyser::Transaction::calcHash()
{
    std::string unhashed = _version + std::to_string(_time);
    std::string hashed;

    for (auto& input : _inputs)
        unhashed.append(input._txHash + std::to_string(input._index));

    for (auto& output : _outputs)
        unhashed.append(std::to_string(output._amount) + output._recipient);

    unhashed += _sig._rSigVal + _sig._sSigVal;

    cryptography::SHA256(unhashed, hashed);

    return hashed;
}

void keyser::Transaction::sign(cryptography::ECKeyPair* signingKey)
{   
    signingKey->sign(calcHash(), _sig._rSigVal, _sig._sSigVal);
}

bool keyser::Transaction::isValid()
{
    // Transaction is not valid if it has not been signed
    if (_sig._rSigVal == "" || _sig._sSigVal == "") {
        std::cout << "No signature found." << std::endl;
        return false;
    }

    for (auto& input : _inputs)
    {
        // Instantiate new ECKeyPair with the senders public key
        cryptography::ECKeyPair keyPair = cryptography::ECKeyPair("public", input._pubKey);

        // Use the instantiated ECKeyPair to verify the transactions signature
        if (!keyPair.verify(calcHash(), _sig._rSigVal, _sig._sSigVal))
        {
            std::cout << "Invalid transaction." << std::endl;
            return false;
        }
    }

    return true;
}

nlohmann::json keyser::Transaction::json() const
{
    nlohmann::json json;

    json["version"] = KEYSER_VERSION;
    json["time"]    = _time;

    json["inputs"] = nlohmann::json::array();
    for (auto& input : _inputs)
        json["inputs"].push_back(input.json());

    json["outputs"] = nlohmann::json::array();
    for (auto& output : _outputs)
        json["outputs"].push_back(output.json());

    json["signature"] = _sig.json();

    return json;
}

void keyser::Transaction::json(nlohmann::json json)
{
    // _time            = json["time"];
    // _amount          = json["amount"];
    // _recieverAddress = json["recieverAddress"];
    // _senderAddress   = json["senderAddress"];
    // _senderPublicKey = json["senderPublicKey"];
    // _hash            = json["hash"];
    // _rSigVal         = json["rSigVal"];
    // _sSigVal         = json["sSigVal"];

    // _version = json["version"];
    // _time = json["time"];

    // for (auto& input : json["inputs"])
    // {

    // }
}

// Operator overloading
namespace keyser
{
//     std::ostream& operator<<(std::ostream& out, Transaction& data) {
//         out << "Time: "              << utils::localTime(data._time) << ", ";
//         out << "Amount: "            << data._amount                 << ", ";
//         out << "Recieving Address: " << data._recieverAddress        << ", ";
//         out << "Sender: "            << data._senderAddress;

//         return out;
//     }

    std::ostream& operator<<(std::ostream& out, Transaction& data) 
    {
        // TODO

        return out;
    }
}
