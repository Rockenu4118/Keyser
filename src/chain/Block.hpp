#ifndef BLOCK_H
#define BLOCK_H

#include <string>
#include <vector>
#include <nlohmann/json.hpp>

#include "./Transaction.hpp"


namespace keyser
{
    struct BlockHeader
    {
        uint        _index;
        std::string _version;
        std::string _prevHash;
        std::string _bodyHash;
        time_t      _time;
        uint64_t    _nonce = 0;
    };

    class Block : public BlockHeader
    {
        // IO Stream operators
        friend std::ostream& operator<<(std::ostream& out, Block& data);

        public:
            // Json parsing
            Block(nlohmann::json json);

            // Main block constructor miners will use to form a block
            Block(uint index, time_t time, std::string prevHash, std::vector<Transaction> transactions);

            ~Block() = default;

            BlockHeader getHeader() const;

            std::string hash() const;

            std::string bodyHash() const;

            // Validate all transactions within block
            bool hasValidTransactions();

            nlohmann::json json() const;

            // Print all transactions within block
            void printTransactions();

            // Body of transactions
            std::vector<Transaction> _transactions;
    };
}

#endif