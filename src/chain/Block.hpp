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
        time_t      _time;
        uint64_t    _nonce;
        std::string _prevHash;
        std::string _hash;
    };

    class Block : public BlockHeader
    {
        // IO Stream operators
        friend std::ostream& operator<<(std::ostream& out, Block& data);

        public:
            // Construct blank block instance for json parsing purposes
            Block() = default;

            Block(nlohmann::json json);

            // Main block constructor miners will use to form a block
            Block(uint index, time_t time, std::string prevHash, std::vector<Transaction> transactions);

            BlockHeader getHeader() const;

            // Calculate the blocks hash
            std::string calcHash();

            // See if hash satisfies difficulty
            bool hasValidHash(uint8_t difficulty);

            // Validate all transactions within block
            bool hasValidTransactions();

            nlohmann::json json() const;

            void json(nlohmann::json json);

            // Print all transactions within block
            void printTransactions();

            // Body of transactions
            std::vector<Transaction> _transactions;
    };
}

#endif