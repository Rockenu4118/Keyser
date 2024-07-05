#include <algorithm>

#include "./Transaction.hpp"
#include "../wallet/Wallet.hpp"
#include "../utils/utils.hpp"
#include "../data/version.hpp"


std::string keyser::OutPoint::calcHash() const
{
    std::string unhashed = _txHash + std::to_string(_index);
    std::string hashed;

    cryptography::SHA256(unhashed, hashed);

    return hashed;
}


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
}

keyser::Transaction::Transaction(uint64_t amount, std::string recipientAddr)
{
    _version = KEYSER_VERSION;
    _time = time(NULL);

    TxOut txout;
    txout._amount    = amount;
    txout._recipient = recipientAddr;

    _outputs.push_back(txout);
}

keyser::Transaction::Transaction(std::vector<UTXO> Utxos, uint64_t amount, uint64_t gas, std::string recipientAddr, std::string sendingPublicKey)
{
    _version = KEYSER_VERSION;
    _time = time(NULL);

    // Sum of all provided Utxos
    uint64_t totalFunds = std::accumulate(Utxos.cbegin(), Utxos.cend(), 0, [](uint64_t sum, UTXO utxo) { return sum += utxo._output._amount; });

    // Sent funds
    TxOut transfer;
    transfer._recipient = recipientAddr;
    transfer._amount = amount;
    _outputs.push_back(transfer);

    // Create an output for change if there are leftover funds
    if ((totalFunds - amount - gas) != 0)
    {
        // Excess funds
        TxOut change;
        change._recipient = utils::pubKeytoAddress(sendingPublicKey);
        change._amount = totalFunds - amount - gas;
        _outputs.push_back(change);
    }

    // Create inputs
    for (auto utxo : Utxos)
    {
        TxIn input;
        input._outPoint = utxo._outPoint;
        input._pubKey = sendingPublicKey;
        _inputs.push_back(input);
    }
}

std::string keyser::Transaction::hash() const
{
    std::string unhashed = _version + std::to_string(_time);
    std::string hashed;

    for (auto& input : _inputs)
        unhashed.append(input._outPoint._txHash + std::to_string(input._outPoint._index) + input._pubKey);

    for (auto& output : _outputs)
        unhashed.append(std::to_string(output._amount) + output._recipient);

    cryptography::SHA256(unhashed, hashed);

    return hashed;
}

void keyser::Transaction::sign(cryptography::ECKeyPair* signingKey)
{
    for (auto& input : _inputs)
    {
        signingKey->sign(hash(), input._sig._rSigVal, input._sig._sSigVal);
    }
}

bool keyser::Transaction::isSigned() const
{
    for (auto input : _inputs)
    {
        if (input._sig._rSigVal == "" || input._sig._sSigVal == "")
            return false;
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

    return json;
}

// Operator overloading
namespace keyser
{
    std::ostream& operator<<(std::ostream& out, TxOut& data)
    {
        out << "Amount: " << data._amount << ", Recipient: " << data._recipient;

        return out;
    }

    std::ostream& operator<<(std::ostream& out, UTXO& data)
    {
        out << "Origin: " << data._outPoint._txHash << ", Index: " << data._outPoint._index << std::endl; 
        out << "Amount: " << data._output._amount << ", Recipient: " << data._output._recipient;

        return out;
    }

    std::ostream& operator<<(std::ostream& out, Transaction& data) 
    {
        out << "Time: "      << utils::localTime(data._time)   << std::endl;
        out << "Inputs: "    << data._inputs.size()            << std::endl;
        out << "Recipient: " << data._outputs.at(0)._recipient << std::endl;
        out << "Amount: "    << data._outputs.at(0)._amount;

        return out;
    }
}
