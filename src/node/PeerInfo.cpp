#include "./PeerInfo.hpp"


keyser::PeerInfo::PeerInfo(nlohmann::json json)
{
    version = json["version"];
    alias   = json["alias"];
    address = json["address"];
    port    = json["port"];
}

namespace keyser
{
    std::ostream& operator<<(std::ostream& out, PeerInfo& data)
    {
        out << "Version: " << data.version << ", ";
        out << "Alias: "   << data.alias   << ", ";
        out << "Address: " << data.address << ", ";
        out << "Port: "    << data.port;

        return out;
    }

    bool operator<(const PeerInfo& lhs, const PeerInfo& rhs)
    {
        return lhs.endpoint() < rhs.endpoint();
    }

    bool operator==(const PeerInfo& lhs, const PeerInfo& rhs)
    {
        return lhs.endpoint() == rhs.endpoint();
    }
}