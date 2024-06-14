#include "./StorageEngine.hpp"
#include "../node/Node.hpp"
#include "../utils/utils.hpp"
#include "../app/Settings.hpp"

#include <cassert>
#include <string>
#include <vector>
#include <memory>
#include <nlohmann/json.hpp>
#include <leveldb/db.h>


keyser::StorageEngine::StorageEngine()
{
    leveldb::Options options;
    options.create_if_missing = true;
    std::string homeDir = getenv("HOME");
    status = leveldb::DB::Open(options, homeDir + "/keyser", &_db);
    status = leveldb::DB::Open(options, homeDir + "/keyser/blocks", &_blocks);
    status = leveldb::DB::Open(options, homeDir + "/keyser/wallets", &_wallets);
    status = leveldb::DB::Open(options, homeDir + "/keyser/peers", &_peers);
}

keyser::StorageEngine::~StorageEngine()
{
    delete _db;
    delete _blocks;
    delete _wallets;
    delete _peers;
}

void keyser::StorageEngine::saveSettings(const Settings& settings)
{
    status = _db->Put(leveldb::WriteOptions(), "settings", settings.json().dump());
}

void keyser::StorageEngine::loadSettings(Settings& settings)
{
    std::string jsonStr;

    status = _db->Get(leveldb::ReadOptions(), "settings", &jsonStr);

    nlohmann::json json = nlohmann::json::parse(jsonStr);

    settings.json(json);
}

void keyser::StorageEngine::saveBlock(const Block& block)
{
    status = _blocks->Put(leveldb::WriteOptions(), block._hash, block.json().dump());
}

void keyser::StorageEngine::loadBlock(std::string hash, Block& block)
{
    std::string jsonStr;

    status = _blocks->Get(leveldb::ReadOptions(), hash, &jsonStr);

    if (jsonStr == "")
        return;

    nlohmann::json json = nlohmann::json::parse(jsonStr);

    block.json(json);
}

void keyser::StorageEngine::saveWallets(const WalletManager& wallets)
{
    status = _wallets->Put(leveldb::WriteOptions(), "wallets", wallets.json().dump());
}

void keyser::StorageEngine::loadWallets(WalletManager& wallets)
{
    std::string jsonStr;

    status = _wallets->Get(leveldb::ReadOptions(), "wallets", &jsonStr);

    if (jsonStr == "")
        return;

    nlohmann::json json = nlohmann::json::parse(jsonStr);

    wallets.json(json);
}

void keyser::StorageEngine::savePeer(const NodeInfo& nodeInfo)
{
    status = _peers->Put(leveldb::WriteOptions(), nodeInfo.endpoint(), nodeInfo.json().dump());
}

void keyser::StorageEngine::loadPeer(NodeInfo& nodeInfo)
{
    std::string jsonStr;

    status = _peers->Get(leveldb::ReadOptions(), nodeInfo.endpoint(), &jsonStr);

    if (jsonStr == "")
        return;

    nlohmann::json json = nlohmann::json::parse(jsonStr);

    // TODO - finish
}