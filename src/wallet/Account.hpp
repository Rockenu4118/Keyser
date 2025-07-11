#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
#include <iostream>
#include <openssl/ec.h>
#include <nlohmann/json.hpp>
#include <keycrypto/Stealth.hpp>


namespace keyser
{
    class Account
    {
    // IO Stream operators
    friend std::ostream& operator<<(std::ostream& out, Account& data);

        public:
            Account() = default;

            explicit Account(nlohmann::json json);

            explicit Account(std::string name);

            Account(std::string name, std::string privateKey);

            ~Account() = default;

            std::string getName() const;

            std::string getPublicAddress() const;

            crypto::StealthKeys* getKeyPair() const;

            // nlohmann::json json() const;

        private:
            std::string           _name;
            crypto::StealthKeys*  _keyPair;
    };
}

#endif