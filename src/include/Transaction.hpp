#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include <iostream>
#include <openssl/ec.h>

#include <cryptography.hpp>

namespace Keyser
{
    class Transaction
    {
        // IO Stream operators
        friend std::ostream& operator<<(std::ostream& out, Transaction& data);

        public:
            // Constructors
            Transaction(int amount, std::string recievingAddress, std::string sendingPublicKey);
            Transaction() = default;

            // Accessors
            uint        getAmount();
            std::string getRecieverAddress();
            std::string getSenderAddress();
            std::string getSenderPublicKey();
            std::string getHash();
            ECDSA_SIG*  getSignature();

            // Modifiers
            void calcHash();
            void sign(cryptography::ECKeyPair* signingKey);

            // Other
            bool isValid();

        private:
            uint        _amount;
            std::string _recieverAddress;
            std::string _senderAddress;
            std::string _senderPublicKey;
            std::string _hash;
            ECDSA_SIG*  _signature;
    };
}

#endif