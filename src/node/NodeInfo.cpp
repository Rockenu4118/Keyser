#include <nlohmann/json.hpp>

#include "./NodeInfo.hpp"


keyser::NodeInfo::NodeInfo(nlohmann::json json)
{
    _version = json["version"];
    _alias   = json["alias"];
    _address = json["address"];
    _port    = json["port"];
}