#include <iostream>
#include <string>

// #include <openssl/ec.h>
#include <cryptography.hpp>

#include "./Transaction.hpp"
#include "./Wallet.hpp"
#include "../utils/utils.hpp"



// Constructors
keyser::Transaction::Transaction(int amount, std::string recievingAddress, std::string sendingPubKey)
{
    _amount          = amount;
    _recieverAddress = recievingAddress;
    _senderAddress   = keyser::utils::pubKeytoAddress(sendingPubKey);
    _senderPublicKey = sendingPubKey;
    _hash            = "";
    _signature       = nullptr;
}

// Accessors
uint keyser::Transaction::getAmount()
{
    return _amount;
}

std::string keyser::Transaction::getRecieverAddress()
{
    return _recieverAddress;
}

std::string keyser::Transaction::getSenderAddress()
{
    return _senderAddress;
}

std::string keyser::Transaction::getSenderPublicKey()
{
    return _senderPublicKey;
}

std::string keyser::Transaction::getHash()
{
    return _hash;
}

ECDSA_SIG* keyser::Transaction::getSignature()
{
    return _signature;
}

// Modifiers
void keyser::Transaction::calcHash()
{
    std::string unhashed = std::to_string(_amount) + _recieverAddress + _senderAddress + _senderPublicKey;
    std::string hashed = "";

    cryptography::SHA256(unhashed, hashed);

    _hash = hashed;
}

void keyser::Transaction::sign(cryptography::ECKeyPair* signingKey)
{   
    if (signingKey->getUPublicKey() != _senderPublicKey) {
        std::cout << "Cannot sign transactions for other wallets." << std::endl;
        return;
    }

    calcHash();

    _signature = signingKey->sign(_hash);
}

bool keyser::Transaction::isValid()
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
namespace keyser
{
    std::ostream& operator<<(std::ostream& out, Transaction& data) {
        out << "Amount: "            << data.getAmount()           << ", ";
        out << "Recieving Address: " << data.getRecieverAddress() << ", ";
        out << "Sender: "            << data.getSenderAddress();

        return out;
    }
}
