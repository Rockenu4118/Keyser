#ifndef BLOCK_H
#define BLOCK_H

#include <string>
#include <vector>

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
        double      _reward;
        std::string _rewardAddress;
    };

    class Block : public BlockHeader
    {
        // IO Stream operators
        friend std::ostream& operator<<(std::ostream& out, Block& data);

        public:
            // Construct blank block instance for json parsing purposes
            Block() = default;

            // Main block constructor miners will use to form a block
            Block(uint index, time_t time, std::string prevHash, double reward, std::string rewardAddress, std::vector<Transaction> transactions);

            // Calculate the blocks hash
            void calcHash();

            // Call calcHash() until a hash is generated that meets the difficulty requirements
            void calcValidHash(uint8_t difficulty);

            // Validate all transactions within block
            bool hasValidTransactions();

            // Print all transactions within block
            void printTransactions();
        
            // Body of transactions
            std::vector<Transaction> _transactions;
    };
}

#endif