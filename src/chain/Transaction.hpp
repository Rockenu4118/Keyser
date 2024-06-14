#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include <vector>
#include <iostream>
#include <nlohmann/json.hpp>

#include <cryptography.hpp>

namespace keyser
{
    // class Transaction
    // {
    //     // IO Stream operators
    //     friend std::ostream& operator<<(std::ostream& out, Transaction& data);

    //     public:
    //         // Constructors
    //         Transaction() = default;
    //         Transaction(nlohmann::json json);
    //         Transaction(double amount, std::string recievingAddress, std::string sendingPublicKey);
            
    //         void calcHash();

    //         void sign(cryptography::ECKeyPair* signingKey);

    //         nlohmann::json json() const;

    //         void json(nlohmann::json json);

    //         // Other
    //         bool isValid();

    //     public:
    //         double      _amount;
    //         time_t      _time;
    //         std::string _recieverAddress;
    //         std::string _senderAddress;
    //         std::string _senderPublicKey;
    //         std::string _hash;
    //         std::string _rSigVal;
    //         std::string _sSigVal;
    // };

    struct TxIn
    {
        TxIn(nlohmann::json json)
        {
            _txHash = json["txHash"];
            _index  = json["index"];
        }

        nlohmann::json json() const
        {
            nlohmann::json json;
            json["txHash"] = _txHash;
            json["index"]  = _index;
            return json;
        }

        std::string _txHash;
        uint        _index;
        std::string _pubKey;
    };

    struct TxOut
    {
        TxOut(nlohmann::json json)
        {
            _amount    = json["amount"];
            _recipient = json["recipient"];
        }

        nlohmann::json json() const 
        {
            nlohmann::json json;
            json["amount"]    = _amount;
            json["recipient"] = _recipient;
            return json;
        }

        double      _amount;
        std::string _recipient;
    };

    struct TxSig
    {
        TxSig() = default;
        TxSig(nlohmann::json json)
        {
            _rSigVal = json["rSigVal"];
            _sSigVal = json["sSigVal"];
        }

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

    struct Transaction
    {
        friend std::ostream& operator<<(std::ostream& out, Transaction& data);

        Transaction() = default;
        Transaction(nlohmann::json json);
        Transaction(double amount, std::string recievingAddress, std::string sendingPublicKey);

        nlohmann::json json() const;

        void json(nlohmann::json json);
            
        std::string calcHash();

        void sign(cryptography::ECKeyPair* signingKey);

        bool isValid();
        
        std::string        _version;
        time_t             _time;
        std::vector<TxIn>  _inputs;
        std::vector<TxOut> _outputs;
        TxSig              _sig;
    };
}

#endif