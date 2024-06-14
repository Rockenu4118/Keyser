#include <string>
#include <nlohmann/json.hpp>

#include "./Settings.hpp"


nlohmann::json keyser::Settings::json() const
{
    nlohmann::json json;

    json["alias"]         = _alias;
    json["tcpPort"]       = _tcpPort;
    json["httpPort"]      = _httpPort;
    json["rewardAddress"] = _rewardAddress;

    return json;
}

void keyser::Settings::json(nlohmann::json json)
{
    _alias         = json["alias"];
    _tcpPort       = json["tcpPort"];
    _httpPort      = json["httpPort"];
    _rewardAddress = json["rewardAddress"];
}

namespace keyser
{
    std::ostream& operator<<(std::ostream& out, Settings& data)
    {
        out << "Alias: "          << data._alias    << std::endl;
        out << "TCP Port: "       << data._tcpPort  << std::endl;
        out << "HTTP Port: "      << data._httpPort << std::endl;
        out << "Reward Address: " << data._rewardAddress;

        return out;
    }
}