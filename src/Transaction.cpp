#include <iostream>
#include <string>

// #include <openssl/ec.h>
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

    _signature = signingKey->sign(_hash);
}

bool Keyser::Transaction::isValid()
{
    // Sender address will be "None" if transaction is from the genesis block or a mining reward
    if (_senderAddress == "None") { 
        std::cout << "Mining reward valid." << std::endl;
        return true;
    }

    // Transaction is not valid if it has not been signed
    if (_signature == nullptr) {
        std::cout << "No signature found." << std::endl;
        return false;
    }

    // Instantiate new ECKeyPair with the senders public key
    cryptography::ECKeyPair keyPair = cryptography::ECKeyPair("public", _senderPublicKey);

    // Use the instantiated ECKeyPair to verify the transactions signature
    if (!keyPair.verify(_hash, _signature)) {
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
