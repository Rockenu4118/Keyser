#include <iostream>
#include <string>

#include <openssl/ec.h>
#include <cryptography.hpp>

#include "./include/Transaction.hpp"
#include "./include/Wallet.hpp"
#include "./utils/utils.hpp"



// Constructors
Keyser::Transaction::Transaction(int amount, std::string recievingAddress, std::string sendingPubKey)
{
    _amount          = amount;
    _recieverAddress = recievingAddress;
    _senderAddress   = keyser::utils::pubKeytoAddress(sendingPubKey);
    _senderPublicKey = sendingPubKey;
    _hash            = "";
    _signature       = nullptr;
}

// Accessors
uint Keyser::Transaction::getAmount()
{
    return _amount;
}

std::string Keyser::Transaction::getRecieverAddress()
{
    return _recieverAddress;
}

std::string Keyser::Transaction::getSenderAddress()
{
    return _senderAddress;
}

std::string Keyser::Transaction::getSenderPublicKey()
{
    return _senderPublicKey;
}

std::string Keyser::Transaction::getHash()
{
    return _hash;
}

ECDSA_SIG* Keyser::Transaction::getSignature()
{
    return _signature;
}

// Modifiers
void Keyser::Transaction::calcHash()
{
    std::string unhashed = std::to_string(_amount) + _recieverAddress + _senderAddress + _senderPublicKey;
    std::string hashed = "";

    cryptography::SHA256(unhashed, hashed);

    _hash = hashed;
}

void Keyser::Transaction::sign(cryptography::ECKeyPair* signingKey)
{   
    if (signingKey->getUPublicKey() != _senderPublicKey) {
        std::cout << "Cannot sign transactions for other wallets." << std::endl;
        return;
    }

    calcHash();

    _signature = ECDSA_do_sign((const unsigned char *)_hash.c_str(), strlen(_hash.c_str()), signingKey->getKeyPairObj());
}

bool Keyser::Transaction::isValid()
{
    if (_senderAddress == "None") { 
        std::cout << "Mining reward valid." << std::endl;
        return true;
    }

    if (_signature == nullptr) {
        std::cout << "No signature found." << std::endl;
        return false;
    }

    cryptography::ECKeyPair keyPair = cryptography::ECKeyPair("public", _senderPublicKey);

    // TODO finish this, last param
    if (!ECDSA_do_verify((const unsigned char *)_hash.c_str(), strlen(_hash.c_str()), _signature, keyPair.getKeyPairObj())) {
        std::cout << "Invalid transaction." << std::endl;
        return false;
    }

    std::cout << "Valid transaction." << std::endl;
    return true;
}

// Operator overloading
namespace Keyser
{
    std::ostream& operator<<(std::ostream& out, Transaction& data) {
        out << "Amount: "            << data.getAmount()           << ", ";
        out << "Recieving Address: " << data.getRecieverAddress() << ", ";
        out << "Sender: "            << data.getSenderAddress();

        return out;
    }
}
