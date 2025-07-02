#include <string>
#include <nlohmann/json.hpp>

#include "./Settings.hpp"

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