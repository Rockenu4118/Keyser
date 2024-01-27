#ifndef NODE_ADDRESSES_H
#define NODE_ADDRESSES_H

#include <vector>
#include <string>

#include "../node/NodeInfo.hpp"

keyser::NodeInfo node1 = {
    "0.1",
    "node1",
    "172.233.157.161",
    6000
};

keyser::NodeInfo node2 = {
    "0.1",
    "node2",
    "172.233.137.160",
    6000
};

std::vector<keyser::NodeInfo> activeNodeList = { node1, node2 };


#endif