// TODO - create a class that houses global node settings
// Load settings from disk into memory on startup
// - Reward address for mining
// - Ports (Node and RPC)
// - Node alias
// - etc.


#ifndef SETTINGS_H
#define SETTINGS_H

#include <iostream>
#include <string>
#include <nlohmann/json.hpp>


namespace keyser
{
    struct Settings
    {   
        // IO Stream operator
        friend std::ostream& operator<<(std::ostream& out, Settings& data);

        // Constructor
        Settings() = default;

        nlohmann::json json() const;

        void json(nlohmann::json json);

        // General info

        std::string _alias;

        // Network

        uint16_t _tcpPort = 6000;
        uint16_t _httpPort = 8080;

        // Mining

        std::string _rewardAddress;
    };
}

#endif
