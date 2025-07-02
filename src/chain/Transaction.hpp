#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include <vector>
#include <iostream>
#include <nlohmann/json.hpp>

#include <keycrypto/sha.hpp>
#include <keycrypto/Stealth.hpp>

namespace keyser
{
    struct Transaction
    {
        uint64_t mVersion{};
        time_t   mTimestamp{};
        uint64_t mLocktime{};
        uint64_t mGas{};
        unsigned char mRecipient[64];
        unsigned char mSender[64];
        uint64_t mAmount{};
    };

    Transaction newTransaction(unsigned char recipient[], unsigned char sender[], uint64_t amount);

    void hashTransaction(Transaction& transaction, unsigned char hash[]);


    // struct OutPoint
    // {
    //     std::string hash() const;
    //
    //     std::string _R;
    //     uint        _index;
    // };

    // struct TxIn
    // {
    //     TxIn() = default;
    //
    //     explicit TxIn(nlohmann::json json)
    //     {
    //         _ringMembers = json["ringMembers"];
    //         _ringSig     = json["ringSig"];
    //         _keyImage    = json["keyImage"];
    //     }
    //
    //     ~TxIn() = default;
    //
    //     nlohmann::json json() const
    //     {
    //         nlohmann::json json;
    //         json["ringMembers"] = _ringMembers;
    //         json["ringSig"]     = _ringSig;
    //         json["keyImage"]    = _keyImage;
    //         return json;
    //     }
    //
    //     std::vector<std::string> _ringMembers;
    //     std::vector<std::string> _ringSig;
    //     std::string              _keyImage;
    // };

    // struct TxOut
    // {
    //     friend std::ostream& operator<<(std::ostream& out, const TxOut& data);
    //
    //     TxOut() = default;
    //
    //     explicit TxOut(nlohmann::json json)
    //     {
    //         _amount    = json["amount"];
    //         _recipient = json["recipient"];
    //     }
    //
    //     ~TxOut() = default;
    //
    //     [[nodiscard]] nlohmann::json json() const
    //     {
    //         nlohmann::json json;
    //         json["amount"]    = _amount;
    //         json["recipient"] = _recipient;
    //         return json;
    //     }
    //
    //     uint64_t    _amount{};
    //     std::string _recipient;
    // };

    // struct TXO
    // {
    //     friend std::ostream& operator<<(std::ostream& out, TXO& data);
    //
    //     OutPoint _outPoint;
    //     TxOut    _output;
    // };

    // struct Transaction
    // {
    //     friend std::ostream& operator<<(std::ostream& out, const Transaction& data);
    //
    //     Transaction() = default;
    //
    //     explicit Transaction(nlohmann::json json);
    //
    //     // Constructor for coinbase transactions
    //     Transaction(uint64_t amount, std::string recipientAddr, uint64_t lockTime);
    //
    //     Transaction(std::vector<TxOut> outputs, uint64_t amount, uint64_t lockTime, uint64_t gas, std::string recipientAddress, std::string changeAddr);
    //
    //     ~Transaction() = default;
    //
    //     nlohmann::json json() const;
    //
    //     std::string hash() const;
    //
    //     // void addInput();
    //
    //     bool sign(crypto::StealthKeys* signingKey);
    //
    //     bool isSigned() const;
    //
    //     std::string        _version;
    //     time_t             _time{};
    //     uint64_t           _lockTime{};
    //     uint64_t           _gas{};
    //     std::vector<TxIn>  _inputs;
    //     std::vector<TxOut> _outputs;
    //     std::string        _extra;
    // };
}

#endif