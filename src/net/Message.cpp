#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <cstring>

#include "./Message.hpp"
#include "../utils/utils.hpp"
#include "./MsgTypes.hpp"
#include "../chain/Block.hpp"
#include "../chain/Transaction.hpp"
#include "../node/NodeInfo.hpp"


keyser::Message::Message(MsgTypes id)
{
    header.id = id;
}

void keyser::Message::serialize()
{
    std::string msg = _doc.dump();
    int         len = msg.size();

    for (int i = 0 ; i < len ; i++)
    {
        body.push_back(msg[i]);
    }

    header.size = size();
}

void keyser::Message::deserialize()
{
    std::string msg;
    int         len = body.size();

    for (int i = 0 ; i < len ; i++)
    {
        msg += body.at(i);
    }

    _doc = nlohmann::json::parse(msg);
}

void keyser::Message::insert(Block& block)
{
    utils::encodeJson(_doc, block);
    serialize();
}

void keyser::Message::insert(Transaction& transaction)
{
    utils::encodeJson(_doc, transaction);
    serialize();
}

void keyser::Message::insert(NodeInfo& nodeInfo)
{
    utils::encodeJson(_doc, nodeInfo);
    serialize();
}

void keyser::Message::extract(Block& block)
{
    deserialize();
    utils::decodeJson(block, _doc);
}

void keyser::Message::extract(Transaction& transaction)
{
    deserialize();
    utils::decodeJson(transaction, _doc);
}

void keyser::Message::extract(NodeInfo& nodeInfo)
{
    deserialize();
    utils::decodeJson(nodeInfo, _doc);
}

size_t keyser::Message::size() const
{
    return body.size();
}

void keyser::Message::print()
{
    for (int i = 0 ; i < header.size ; i++)
    {
        std::cout << body.at(i);
    }

    std::cout << std::endl;
}

