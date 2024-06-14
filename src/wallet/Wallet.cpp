#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
#include <openssl/evp.h>
#include <openssl/ec.h>

#include "./Wallet.hpp"
#include "../utils/utils.hpp"
#include <cryptography.hpp>


keyser::Wallet::Wallet()
{
    _name          = "";
    _keyPair       = nullptr;
    _publicAddress = "";
}

keyser::Wallet::Wallet(nlohmann::json json)
{
    _name          = json["name"];
    _publicAddress = json["publicAddress"];
    _keyPair       = new cryptography::ECKeyPair(json["keyPair"]);
}

keyser::Wallet::Wallet(std::string name)
{    
    _name = name;

    // Generate EC Key Pair object
    _keyPair = new cryptography::ECKeyPair();

    // Calculate public address with previously generated EC uncompressed public key
    _publicAddress = keyser::utils::pubKeytoAddress(_keyPair->getUPublicKey());
}

keyser::Wallet::Wallet(std::string name, std::string privateKey)
{
    _name = name;

    // Insert provided private key
    _keyPair = new cryptography::ECKeyPair("private", privateKey);

    // Calculate public address with previously provided private key
    _publicAddress = keyser::utils::pubKeytoAddress(_keyPair->getUPublicKey());
}

std::string keyser::Wallet::getPublicAddress() const
{
    return _publicAddress;
}

std::string keyser::Wallet::getName() const
{
    return _name;
}

cryptography::ECKeyPair* keyser::Wallet::getKeyPair() const
{
    return _keyPair;
}

nlohmann::json keyser::Wallet::json() const
{
    nlohmann::json json;

    json["name"]          = _name;
    json["publicAddress"] = _publicAddress;
    json["keyPair"]       = _keyPair->json();

    return json;
}

// IO Stream operators
namespace keyser
{
    std::ostream& operator<<(std::ostream& out, Wallet& data) {
        out << "Owner: "          << data.getName() << ", ";
        out << "Public address: " << data.getPublicAddress();

        return out;
    }
}