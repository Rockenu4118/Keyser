#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include <vector>
#include <iostream>
#include <nlohmann/json.hpp>

#include <cryptography.hpp>

namespace keyser
{
    struct TxSig
    {
        TxSig() = default;

        TxSig(nlohmann::json json)
        {
            _rSigVal = json["rSigVal"];
            _sSigVal = json["sSigVal"];
        }

        ~TxSig() = default;

        nlohmann::json json() const
        {
            nlohmann::json json;
            json["rSigVal"] = _rSigVal;
            json["sSigVal"] = _sSigVal;
            return json;
        }

        std::string _rSigVal;
        std::string _sSigVal;
    };

    struct OutPoint
    {
        std::string calcHash() const;

        std::string _txHash;
        uint        _index;
    };

    struct TxIn
    {
        TxIn() = default;

        TxIn(nlohmann::json json) : _sig(json["sig"])
        {
            _outPoint._txHash = json["txHash"];
            _outPoint._index  = json["index"];
            _pubKey = json["pubKey"];
        }

        ~TxIn() = default;

        nlohmann::json json() const
        {
            nlohmann::json json;
            json["txHash"] = _outPoint._txHash;
            json["index"]  = _outPoint._index;
            json["sig"]    = _sig.json();
            json["pubKey"] = _pubKey;
            return json;
        }

        OutPoint    _outPoint;
        TxSig       _sig;
        std::string _pubKey;
    };

    struct TxOut
    {
        friend std::ostream& operator<<(std::ostream& out, TxOut& data);

        TxOut() = default;

        TxOut(nlohmann::json json)
        {
            _amount    = json["amount"];
            _recipient = json["recipient"];
        }

        ~TxOut() = default;

        nlohmann::json json() const 
        {
            nlohmann::json json;
            json["amount"]    = _amount;
            json["recipient"] = _recipient;
            return json;
        }

        uint64_t    _amount;
        std::string _recipient;
    };

    struct UTXO
    {
        friend std::ostream& operator<<(std::ostream& out, UTXO& data);

        OutPoint _outPoint;
        TxOut    _output;
    };

    struct Transaction
    {
        friend std::ostream& operator<<(std::ostream& out, Transaction& data);

        Transaction() = default;

        Transaction(nlohmann::json json);

        // Constructor for coinbase transactions
        Transaction(uint64_t amount, std::string recipientAddr);

        Transaction(std::vector<UTXO> Utxos, uint64_t amount, uint64_t gas, std::string recipientAddress, std::string sendingPublicKey);

        ~Transaction() = default;

        nlohmann::json json() const;
            
        std::string hash() const;

        void sign(cryptography::ECKeyPair* signingKey);

        bool isSigned() const;
        
        std::string        _version;
        time_t             _time;
        std::vector<TxIn>  _inputs;
        std::vector<TxOut> _outputs;
    };
}

#endif