#include <iostream>
#include <string>

#include "./include/Transaction.hpp"
#include "./include/cryptography.hpp"


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

std::string Keyser::Transaction::getHash()
{
    return _hash;
}

// Modifiers
void Keyser::Transaction::calcHash()
{
    std::string unhashed = std::to_string(_amount) + _payer + _payee;
    std::string hashed = "";

    cryptography::sha256(unhashed, hashed);

    _hash = hashed;
}

void Keyser::Transaction::signTransaction()
{
    
}