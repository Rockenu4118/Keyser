#include <algorithm>

#include "./Transaction.hpp"
#include "../wallet/Wallet.hpp"
#include "../utils/utils.hpp"
#include "../data/version.hpp"


std::string keyser::OutPoint::hash() const
{
    return crypto::hash::SHA3(utils::hexToString(_R) + std::to_string(_index));
}

keyser::Transaction::Transaction(nlohmann::json json)
{
    _version = json["version"];
    _time    = json["time"];
    _lockTime= json["lockTime"];
    _gas     = json["gas"];
    _extra   = json["extra"];

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

keyser::Transaction::Transaction(uint64_t amount, std::string recipientAddr, uint64_t lockTime)
{
    _version  = KEYSER_VERSION;
    _time     = time(nullptr);
    _lockTime = lockTime;
    _gas      = 0;

    std::string r = crypto::StealthKeys::genTxPrivKey();
    std::string R = crypto::StealthKeys::deriveTxPubKey(r);

    TxOut txout;
    txout._amount    = amount;
    txout._recipient = crypto::StealthKeys::genStealthAddr(recipientAddr, 0, r);

    _outputs.push_back(txout);

    _extra = R;
}

keyser::Transaction::Transaction(std::vector<TxOut> outputs, uint64_t amount, uint64_t lockTime, uint64_t gas, std::string recipientAddr, std::string changeAddr)
{
    _version = KEYSER_VERSION;
    _time = time(nullptr);
    _lockTime = lockTime;
    _gas = gas;

    // Sum of all provided Utxos
    uint64_t totalFunds = std::accumulate(outputs.cbegin(), outputs.cend(), 0, [](uint64_t sum, const TxOut& txout) { return sum += txout._amount; });

    if (totalFunds < (amount + gas))
        throw std::runtime_error("Insufficient funds to initalize tx!");

    std::string r = crypto::StealthKeys::genTxPrivKey();
    std::string R = crypto::StealthKeys::deriveTxPubKey(r);

    // Sent funds
    TxOut transfer;
    transfer._recipient = crypto::StealthKeys::genStealthAddr(recipientAddr, 0, r);
    transfer._amount = amount;
    _outputs.push_back(transfer);

    // Create an output for change if there are leftover funds
    if ((totalFunds - amount - gas) != 0)
    {
        // Excess funds
        TxOut change;
        change._recipient = crypto::StealthKeys::genStealthAddr(changeAddr, 1, r);
        change._amount = totalFunds - amount - gas;
        _outputs.push_back(change);
    }
}

std::string keyser::Transaction::hash() const
{
    std::string unhashed = _version + std::to_string(_time);
    std::string hashed;

    // for (auto& input : _inputs)
    //     unhashed.append(input._outPoint._txHash + std::to_string(input._outPoint._index) + input._pubKey);

    for (auto& output : _outputs)
        unhashed.append(std::to_string(output._amount) + output._recipient);

    hashed = crypto::hash::SHA3(unhashed);

    return hashed;
}

bool keyser::Transaction::sign(crypto::StealthKeys* signingKey)
{
    // for (auto& input : _inputs)
    // {
    //     // signingKey->sign(hash(), input._sig._rSigVal, input._sig._sSigVal);
    // }



    return false;
}

bool keyser::Transaction::isSigned() const
{
    for (const auto& input : _inputs)
    {
        if (input._ringSig.size() != input._ringMembers.size() + 1)
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
    std::ostream& operator<<(std::ostream& out, const TxOut& data)
    {
        out << "Amount: " << data._amount << ", Recipient: " << data._recipient;

        return out;
    }

    std::ostream& operator<<(std::ostream& out, const Transaction& data)
    {
        out << "Time: "      << utils::localTime(data._time)   << std::endl;
        out << "Inputs: "    << data._inputs.size()            << std::endl;
        out << "Recipient: " << data._outputs.at(0)._recipient << std::endl;
        out << "Amount: "    << data._outputs.at(0)._amount;

        return out;
    }
}
