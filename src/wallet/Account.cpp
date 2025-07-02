#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
#include <openssl/evp.h>
#include <openssl/ec.h>

#include "./Wallet.hpp"
#include "../utils/utils.hpp"



keyser::Account::Account(nlohmann::json json)
{
    _name          = json["name"];
    // _keyPair       = new crypto::StealthKeys(json["keyPair"]);
}

keyser::Account::Account(std::string name)
{    
    _name = name;

    // Generate EC Key Pair object
    // _keyPair = new crypto::StealthKeys();
}

keyser::Account::Account(std::string name, std::string privateKey)
{
    _name = name;

    // Insert provided private key
    // _keyPair = new crypto::StealthKeys(privateKey);
}

// std::string keyser::Account::getPublicAddress() const
// {
//     return _keyPair->getPublicAddr();
// }

std::string keyser::Account::getName() const
{
    return _name;
}

crypto::StealthKeys* keyser::Account::getKeyPair() const
{
    return _keyPair;
}

// nlohmann::json keyser::Account::json() const
// {
//     nlohmann::json json;
//
//     json["name"]          = _name;
//     // json["keyPair"]       = _keyPair->json();
//
//     return json;
// }

// IO Stream operators
namespace keyser
{
    // std::ostream& operator<<(std::ostream& out, Account& data) {
    //     out << "Owner: "          << data.getName() << ", ";
    //     out << "Public address: " << data.getPublicAddress();
    //
    //     return out;
    // }
}