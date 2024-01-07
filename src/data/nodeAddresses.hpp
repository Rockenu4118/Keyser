#ifndef NODE_ADDRESSES_H
#define NODE_ADDRESSES_H

#include <vector>
#include <string>

#include "../node/NodeInfo.hpp"

keyser::NodeInfo node = {
    "0.1",
    "home",
    "127.0.0.1",
    6000
};

keyser::NodeInfo node1 = {
    "0.1",
    "node1",
    "172.233.157.161",
    6000
};

std::vector<keyser::NodeInfo> activeNodeList = { node };


#endif