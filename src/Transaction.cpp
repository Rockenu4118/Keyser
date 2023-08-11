#include <iostream>
#include <string>

#include <openssl/ec.h>

#include "./include/Transaction.hpp"
#include "./include/cryptography.hpp"
#include "./include/Wallet.hpp"
#include "./include/ECKeyPair.hpp"


// Constructors
Keyser::Transaction::Transaction(int amount, std::string reciever, std::string sender)
{
    _amount = amount;
    _reciever = reciever;
    _sender = sender;
}

// Accessors
uint Keyser::Transaction::getAmount()
{
    return _amount;
}

std::string Keyser::Transaction::getReciever()
{
    return _reciever;
}

std::string Keyser::Transaction::getSender()
{
    return _sender;
}

std::string Keyser::Transaction::getHash()
{
    return _hash;
}

// Modifiers
void Keyser::Transaction::calcHash()
{
    std::string unhashed = std::to_string(_amount) + _reciever + _sender;
    std::string hashed = "";

    cryptography::sha256(unhashed, hashed);

    _hash = hashed;
}

void Keyser::Transaction::signTransaction(ECKeyPair* signingKey)
{   
    if (cryptography::pubKeytoAddress(signingKey->getUPublicKey()) != _sender) {
        std::cout << "Cannot sign transactions for other wallets." << std::endl;
        return;
    }

    calcHash();

    _signature = ECDSA_do_sign((const unsigned char *)_hash.c_str(), strlen(_hash.c_str()), signingKey->getKeyPairObj());
}

// Operator overloading
namespace Keyser
{
    std::ostream& operator<<(std::ostream &out, Transaction& data) {
        out << "Amount: "   << data.getAmount()   << ", ";
        out << "Reciever: " << data.getReciever() << ", ";
        out << "Sender: "   << data.getSender();

        return out;
    }
}
