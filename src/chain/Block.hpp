#ifndef BLOCK_H
#define BLOCK_H

#include <string>
#include <vector>

#include "./Transaction.hpp"

namespace keyser
{
    class Block
    {
        // IO Stream operators
        friend std::ostream& operator<<(std::ostream& out, Block& data);

        public:
            // Constructors
            Block() = default;
            Block(uint index, time_t time, std::string prevHash, std::vector<Transaction> transactions);

            // Modifiers
            void calcHash();
            void calcValidHash(uint8_t difficulty);

            // Other
            bool hasValidTransactions();
            void printTransactions();
        
        public:
            uint        _index;
            time_t      _time;
            uint32_t    _nonce;
            std::string _prevHash;
            std::string _hash;

            std::vector<Transaction> _transactions;
    };
}

#endif