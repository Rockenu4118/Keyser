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
        uint          mIndex{};
        uint64_t      mVersion{};
        unsigned char mPrevHash[SHA256_DIGEST_LENGTH];
        unsigned char mMerkleRoot[SHA256_DIGEST_LENGTH];
        Transaction   mReward{};
        time_t        mTime{};
        uint64_t      mNonce{};
    };

    void hashHeader(BlockHeader& header, char hash[]);



    struct Block : BlockHeader
    {
        // IO Stream operators
        friend std::ostream& operator<<(std::ostream& out, const Block& data);

        // Main block constructor miners will use to form a block
        Block(uint index, time_t time, unsigned char prevHash[], Transaction reward, std::vector<Transaction> transactions);

        BlockHeader getHeader() const;
            // Validate all transactions within block
            // bool hasValidTransactions();

        // Body of transactions
        std::vector<Transaction> mTransactions;
    };

    void calcMerkleRoot(std::vector<Transaction>& transactions, unsigned char merkleRoot[]);
}

#endif