#ifndef NODE_INFO_H
#define NODE_INFO_H

#include <string>


namespace keyser
{
    struct NodeInfo
    {
        // Need to overload < operator to be compatible with std::set
        friend bool operator<(const NodeInfo& lhs, const NodeInfo& rhs) 
        {
            return lhs._address < rhs._address;
        }

        std::string _address;
        uint16_t    _port;
    };
}

#endif