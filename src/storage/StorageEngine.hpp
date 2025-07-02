#ifndef STORAGE_ENGINE_H
#define STORAGE_ENGINE_H

#include <vector>
#include <memory>
#include <deque>
// #include <leveldb/db.h>

#include "../app/Settings.hpp"
#include "../chain/Block.hpp"
#include "../wallet/Wallet.hpp"


// namespace keyser
// {
//     class Wallet;

//     class StorageEngine
//     {
//         public:
//             StorageEngine();
            
//             ~StorageEngine();

//             void saveSettings(const Settings& settings);
//             void loadSettings(Settings& settings);

//             void saveBlock(const Block& block);
//             void loadBlock(std::string hash, Block& block);

//             void saveWallets(const Wallet& wallet);
//             void loadWallets(Wallet& wallet);

//             void savePeer(const NodeInfo& nodeInfo);
//             void loadPeer(NodeInfo& nodeInfo);

//         private:
//             leveldb::Status status;

//             leveldb::DB* _db;
//             leveldb::DB* _blocks;
//             leveldb::DB* _wallets;
//             leveldb::DB* _peers;

//     };
// }

#endif