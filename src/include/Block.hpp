#ifndef BLOCK_H
#define BLOCK_H

#include <string>
#include <vector>

#include "./Transaction.hpp"

namespace Keyser
{
    class Block
    {
        // IO Stream operators
        friend std::ostream& operator<<(std::ostream& out, Block& data);

        public:
            // Constructors
            Block(uint index, Block* prevBlock, std::string prevHash, std::vector<Transaction> transactions);

            // Accessors
            uint        getIndex();
            Block*      getPrevBlock();
            std::string getPrevHash();
            time_t      getTime();
            uint32_t    getNonce();
            std::string getHash();

            std::vector<Transaction> getTransactions();

            // Modifiers
            void calcHash();
            void calcValidHash(uint8_t difficulty);

            // Other
            void printTransactions();
        
        private:
            Block*      _prevBlock;
            uint        _index;
            time_t      _time;
            uint32_t    _nonce;
            std::string _prevHash;
            std::string _hash;

            std::vector<Transaction> _transactions;
        
    };
}

#endif