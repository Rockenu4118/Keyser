#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include <iostream>
#include <openssl/ec.h>

#include "./ECKeyPair.hpp"

namespace Keyser
{
    class Transaction
    {
        // IO Stream operators
        friend std::ostream& operator<<(std::ostream& out, Transaction& data);

        public:
            // Constructors
            Transaction(int amount, std::string reciever, std::string sender);
            Transaction() = default;

            // Accessors
            uint        getAmount();
            std::string getReciever();
            std::string getSender();
            std::string getHash();

            // Modifiers
            void calcHash();
            void signTransaction(ECKeyPair* signingKey);

        private:
            uint        _amount;
            std::string _reciever;
            std::string _sender;
            std::string _hash;
            ECDSA_SIG*  _signature;
    };
}

#endif