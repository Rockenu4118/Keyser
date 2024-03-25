#ifndef NODE_INFO_H
#define NODE_INFO_H

#include <string>


namespace keyser
{
    struct NodeInfo
    {
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
            return (lhs._address + std::to_string(lhs._port)) < (rhs._address + std::to_string(rhs._port));
        }

        friend bool operator==(const NodeInfo& lhs, const NodeInfo& rhs)
        {
            return (lhs._address == rhs._address) && (lhs._port == rhs._port);
        }

        enum class Direction : uint8_t
        {
            Outbound,
            Inbound
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