#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>

#include <openssl/ec.h>


namespace Keyser
{
    class Transaction
    {
        public:
            // Constructors
            Transaction(int amount, std::string payer, std::string payee);
            Transaction() = default;

            // Accessors
            uint        getAmount();
            std::string getPayer();
            std::string getPayee();
            std::string getHash();

            // Modifiers
            void calcHash();
            void signTransaction(EC_KEY* signingKey);

        private:
            uint        _amount;
            std::string _payer;
            std::string _payee;
            std::string _hash;
            ECDSA_SIG*  _signature;
    };
}

#endif