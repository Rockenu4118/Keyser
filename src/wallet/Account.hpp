#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
#include <iostream>
#include <memory>
#include <openssl/ec.h>
#include <nlohmann/json.hpp>
#include <cryptography.hpp>


namespace keyser
{
    class Account
    {
    // IO Stream operators
    friend std::ostream& operator<<(std::ostream& out, Account& data);

        public:
            Account() = default;

            Account(nlohmann::json json);

            Account(std::string name);

            Account(std::string name, std::string privateKey);

            ~Account() = default;

            std::string getName() const;

            std::string getPublicAddress() const;

            cryptography::ECKeyPair* getKeyPair() const;

            nlohmann::json json() const;

        private:
            std::string               _name;
            std::string               _publicAddress;
            cryptography::ECKeyPair*  _keyPair;
    };
}

#endif