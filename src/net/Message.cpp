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

void keyser::Message::preparePayload()
{
    std::string msg = _doc.dump();
    int         len = msg.size();

    for (int i = 0 ; i < len ; i++)
    {
        body.push_back(msg[i]);
    }

    header.size = size();
}

void keyser::Message::unpackPayload()
{
    std::string msg;
    int         len = body.size();

    for (int i = 0 ; i < len ; i++)
    {
        msg += body.at(i);
    }

    _doc = nlohmann::json::parse(msg);
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

