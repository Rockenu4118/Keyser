#include <iostream>
#include <string>

#include <cryptography.hpp>

#include "./Transaction.hpp"
#include "../wallet/Wallet.hpp"
#include "../utils/utils.hpp"



// Constructors
keyser::Transaction::Transaction(double amount, std::string recievingAddress, std::string sendingPubKey)
{
    _time            = time(NULL);
    _amount          = amount;
    _recieverAddress = recievingAddress;
    _senderAddress   = keyser::utils::pubKeytoAddress(sendingPubKey);
    _senderPublicKey = sendingPubKey;
    _hash            = "";
    _rSigVal         = "";
    _sSigVal         = "";
}

// Modifiers
void keyser::Transaction::calcHash()
{
    std::string unhashed = std::to_string(_time) + std::to_string(_amount) + _recieverAddress + _senderAddress + _senderPublicKey;
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

    signingKey->sign(_hash, _rSigVal, _sSigVal);
}

bool keyser::Transaction::isValid()
{
    // Transaction is not valid if it has not been signed
    if (_rSigVal == "" || _sSigVal == "") {
        std::cout << "No signature found." << std::endl;
        return false;
    }

    // Instantiate new ECKeyPair with the senders public key
    cryptography::ECKeyPair keyPair = cryptography::ECKeyPair("public", _senderPublicKey);

    // Use the instantiated ECKeyPair to verify the transactions signature
    if (!keyPair.verify(_hash, _rSigVal, _sSigVal)) {
        std::cout << "Invalid transaction." << std::endl;
        return false;
    }

    return true;
}

// Operator overloading
namespace keyser
{
    std::ostream& operator<<(std::ostream& out, Transaction& data) {
        out << "Time: "              << utils::localTime(data._time) << ", ";
        out << "Amount: "            << data._amount                 << ", ";
        out << "Recieving Address: " << data._recieverAddress        << ", ";
        out << "Sender: "            << data._senderAddress;

        return out;
    }
}
