#ifndef PEER_INFO_H
#define PEER_INFO_H

#include <string>
#include <nlohmann/json.hpp>


namespace keyser
{
    struct Endpoint
    {
        friend bool operator==(const Endpoint& lhs, const Endpoint& rhs);

        Endpoint() = default;

        std::string string();

        std::string address;
        uint16_t    port;
    };

    struct NodeInfo
    {
        NodeInfo() = default;

        NodeInfo(nlohmann::json json);

        nlohmann::json json() const;

        std::string version;
        std::string services;
        Endpoint    endpoint;
    };

    struct PeerInfo : public NodeInfo
    {
        enum class Direction : uint8_t
        {
            Outbound,
            Inbound
        };

        // IO Stream operators
        friend std::ostream& operator<<(std::ostream& out, PeerInfo& data);

        friend bool operator==(const PeerInfo& lhs, const PeerInfo& rhs);

        PeerInfo() = default;

        PeerInfo(nlohmann::json json);

        nlohmann::json json() const;

        uint16_t    id;
        Direction   direction;
    };
}

#endif