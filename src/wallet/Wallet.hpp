#ifndef WALLET_H
#define WALLET_H

#include <string>
#include <unordered_map>
#include <nlohmann/json.hpp>

#include "../node/Node.hpp"
#include "./Account.hpp"

namespace keyser
{
    class Wallet
    {
        public:
            Wallet(Node* node);

            ~Wallet() = default;

            void importAccount(std::string name, std::string privKey);

            void createAccount(std::string name);

            const Account& get(std::string name);

            bool createTransaction(int64_t amount, std::string recipient, Account sender);

            nlohmann::json json() const;

            void json(nlohmann::json json);

            void displayAccounts();

            size_t count() const;

        private:
            Node* _node;

            std::unordered_map<std::string, Account> _accounts;
    };
}

#endif