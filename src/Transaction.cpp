#include <iostream>
#include <string>

#include "./include/Transaction.hpp"


// Constructors
Keyser::Transaction::Transaction(int amount, std::string payer, std::string payee)
{
    _amount = amount;
    _payer = payer;
    _payee = payee;
}

// Accessors
uint Keyser::Transaction::getAmount()
{
    return _amount;
}

std::string Keyser::Transaction::getPayer()
{
    return _payer;
}

std::string Keyser::Transaction::getPayee()
{
    return _payee;
}
