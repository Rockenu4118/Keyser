#include "./PeerInfo.hpp"


std::string keyser::Endpoint::string()
{
    std::string ep = address + std::to_string(port);
    return ep;
}

keyser::NodeInfo::NodeInfo(nlohmann::json json)
{
    version          = json["version"];
    endpoint.address = json["address"];
    endpoint.port    = json["port"];
}

nlohmann::json keyser::NodeInfo::json() const
{
    nlohmann::json json;

    json["version"] = version;
    json["address"] = endpoint.address;
    json["port"]    = endpoint.port;

    return json;
}

keyser::PeerInfo::PeerInfo(nlohmann::json json)
{
    version          = json["version"];
    endpoint.address = json["address"];
    endpoint.port    = json["port"];
}

nlohmann::json keyser::PeerInfo::json() const
{
    nlohmann::json json;

    json["version"] = version;
    json["address"] = endpoint.address;
    json["port"]    = endpoint.port;

    return json;
}

namespace keyser
{
    bool operator==(const Endpoint& lhs, const Endpoint& rhs)
    {
        return lhs.address + std::to_string(lhs.port) == rhs.address + std::to_string(rhs.port);
    }


    std::ostream& operator<<(std::ostream& out, PeerInfo& data)
    {
        out << "Version: " << data.version << ", ";
        out << "Address: " << data.endpoint.address << ", ";
        out << "Port: "    << data.endpoint.port;

        return out;
    }

    bool operator==(const PeerInfo& lhs, const PeerInfo& rhs)
    {
        return lhs.endpoint == rhs.endpoint;
    }
}