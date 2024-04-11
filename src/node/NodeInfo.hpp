#ifndef NODE_INFO_H
#define NODE_INFO_H

#include <string>


namespace keyser
{
    struct NodeInfo
    {
        enum class Direction : uint8_t
        {
            Outbound,
            Inbound
        };

        // IO Stream operators
        friend std::ostream& operator<<(std::ostream& out, NodeInfo& data)
        {
            out << "Version: " << data._version << ", ";
            out << "Alias: "   << data._alias   << ", ";
            out << "Address: " << data._address << ", ";
            out << "Port: "    << data._port;

            return out;
        }

        // Need to overload < operator to be compatible with std::set
        friend bool operator<(const NodeInfo& lhs, const NodeInfo& rhs) 
        {
            return lhs.endpoint() < rhs.endpoint();
        }

        friend bool operator==(const NodeInfo& lhs, const NodeInfo& rhs)
        {
            return lhs.endpoint() == rhs.endpoint();
        }

        std::string endpoint() const
        { 
            std::string endpoint = _address + std::to_string(_port);
            return endpoint;
        };

        uint id;

        std::string _version;
        std::string _alias;
        std::string _address;
        uint16_t    _port;

        Direction direction;

        uint startingHeight;
    };
}

#endif