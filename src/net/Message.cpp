#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <cstring>

#include "../utils/utils.hpp"
#include "./Connection.hpp"
#include "./MsgTypes.hpp"
#include "../chain/Block.hpp"
#include "../chain/Transaction.hpp"
#include "../node/NodeInfo.hpp"



keyser::Message::Message(MsgTypes id)
{
    header.id = id;
}



void keyser::Message::push(const std::string& msg)
{
    int len = msg.size();

    for (int i = 0 ; i < len ; i++)
    {
        body.push_back(msg[i]);
    }

    header.size = size();
}

void keyser::Message::pull(std::string& msg)
{
    int len = body.size();

    for (int i = 0 ; i < len ; i++)
    {
        msg += body.at(i);
    }
}

void keyser::Message::serialize()
{
    push(doc.dump());
}

void keyser::Message::deserialize()
{
    std::string msg;
    pull(msg);
    doc = nlohmann::json::parse(msg);
}

void keyser::Message::edit(std::string key, std::string value)
{
    doc[key] = value;
}

void keyser::Message::edit(std::string key, int value)
{
    doc[key] = value;
}

void keyser::Message::edit(std::string key, double value)
{
    doc[key] = value;
}

void keyser::Message::edit(std::string key, time_t value)
{
    doc[key] = value;
}

void keyser::Message::get(std::string key, std::string& value)
{
    value = doc[key];
}

void keyser::Message::get(std::string key, int& value)
{
    value = doc[key];
}

void keyser::Message::get(std::string key, uint16_t& value)
{
    value = doc[key];
}

void keyser::Message::get(std::string key, double& value)
{
    value = doc[key];
}

void keyser::Message::get(std::string key, time_t& value)
{
    value = doc[key];
}

void keyser::Message::insert(Block& block)
{
    std::string str;
    utils::encodeJson(str, block);
    push(str);
}

void keyser::Message::insert(Transaction& transaction)
{
    std::string str;
    utils::encodeJson(str, transaction);
    push(str);
}

void keyser::Message::insert(NodeInfo& nodeInfo)
{
    std::string str;
    utils::encodeJson(str, nodeInfo);
    push(str);
}

void keyser::Message::extract(Block& block)
{
    std::string str;
    pull(str);
    utils::decodeJson(block, str);
}

void keyser::Message::extract(Transaction& transaction)
{
    std::string str;
    pull(str);
    utils::decodeJson(transaction, str);
}

void keyser::Message::extract(NodeInfo& nodeInfo)
{
    std::string str;
    pull(str);
    utils::decodeJson(nodeInfo, str);
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

