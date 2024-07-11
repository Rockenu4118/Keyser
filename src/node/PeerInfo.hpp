#ifndef PEER_INFO_H
#define PEER_INFO_H

#include <string>
#include <nlohmann/json.hpp>


namespace keyser
{
    struct Endpoint
    {
        std::string address;
        uint16_t    port;
    };

    struct PeerInfo
    {
        enum class Direction : uint8_t
        {
            Outbound,
            Inbound
        };

        // IO Stream operators
        friend std::ostream& operator<<(std::ostream& out, PeerInfo& data);

        // Need to overload < operator to be compatible with std::set
        friend bool operator<(const PeerInfo& lhs, const PeerInfo& rhs);

        friend bool operator==(const PeerInfo& lhs, const PeerInfo& rhs);

        PeerInfo() = default;

        PeerInfo(nlohmann::json json);

        std::string endpoint() const
        { 
            std::string endpoint = address + ":" + std::to_string(port);
            return endpoint;
        };

        nlohmann::json json() const
        {
            nlohmann::json json;

            json["version"] = version;
            json["alias"]   = alias;
            json["address"] = address;
            json["port"]    = port;

            return json;
        }

        uint16_t    id;
        Direction   direction;
        std::string version;
        std::string alias;
        std::string address;
        uint16_t    port;
    };
}

#endif