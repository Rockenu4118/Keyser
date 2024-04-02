#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include <vector>
#include <iostream>
#include <openssl/ec.h>

#include <cryptography.hpp>

namespace keyser
{
    class Transaction
    {
        // IO Stream operators
        friend std::ostream& operator<<(std::ostream& out, Transaction& data);

        public:
            // Constructors
            Transaction(double amount, std::string recievingAddress, std::string sendingPublicKey);
            Transaction(double amount, std::string msg, std::string recievingAddress, std::string sendingPublicKey);
            Transaction() = default;

            // Modifiers
            void calcHash();
            void sign(cryptography::ECKeyPair* signingKey);

            // Other
            bool isValid();

        public:
            double      _amount;
            std::string _msg;
            time_t      _time;
            std::string _recieverAddress;
            std::string _senderAddress;
            std::string _senderPublicKey;
            std::string _hash;
            std::string _rSigVal;
            std::string _sSigVal;
    };
}

#endif