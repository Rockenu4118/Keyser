#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>


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

        private:
            uint        _amount;
            std::string _payer;
            std::string _payee;

    };
}

#endif