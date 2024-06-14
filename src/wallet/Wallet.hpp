#ifndef WALLET_H
#define WALLET_H

#include <string>
#include <iostream>
#include <openssl/ec.h>
#include <nlohmann/json.hpp>
#include <cryptography.hpp>


namespace keyser
{
    class Wallet
    {
    // IO Stream operators
    friend std::ostream& operator<<(std::ostream& out, Wallet& data);

        public:
            Wallet();
            Wallet(nlohmann::json json);
            Wallet(std::string name);
            Wallet(std::string name, std::string privateKey);

            std::string              getName() const;
            std::string              getPublicAddress() const;
            cryptography::ECKeyPair* getKeyPair() const;

            nlohmann::json json() const;

        private:
            std::string               _name;
            std::string               _publicAddress;
            cryptography::ECKeyPair*  _keyPair;
    };
}

#endif