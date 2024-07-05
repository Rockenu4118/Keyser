#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <ctime>
#include <algorithm>
#include <cryptography.hpp>
#include <nlohmann/json.hpp>

#include "./utils.hpp"
#include "../wallet/Wallet.hpp"
#include "../chain/Transaction.hpp"
#include "../chain/Block.hpp"
#include "../node/NodeInfo.hpp"


std::string keyser::utils::pubKeytoAddress(const std::string& uPublicKey)
{   
    // Step 1: Convert the raw hex characters of the uncompressed public key to a string.
    std::string unhashed = hexToString(uPublicKey);
    std::string hashed = "";

    // Step 2: Generate a sha256 hash of the uncompressed public key.
    cryptography::SHA256(unhashed, hashed);

    // Step 3: Extract last 32 (hex) characters, 16 bytes, of the sha256 hash.
    std::string address = hashed.substr(32, 32);

    // Step 4: Prepend "0x" to indicate that the address is in hex format.
    std::string publicAddress = "0x" + address;

    return publicAddress;
}

std::string keyser::utils::hexToString(const std::string& input)
{
    std::string output;
    if ((input.length() % 2) != 0) {
        std::cout << "String is not valid length." << std::endl;
    }
    size_t count = input.length() / 2;
    for (size_t i = 0 ; count > i ; i++) {
        uint32_t s = 0;
        std::stringstream ss;
        ss << std::hex << input.substr(i * 2, 2);
        ss >> s;
        output.push_back(static_cast<unsigned char>(s));
    }
    return output;
}

bool keyser::utils::isValidHash(std::string hash, uint8_t difficulty)
{
    for (uint i = 0 ; i < difficulty ; i++)
    {
        if (hash[i] != '0') 
        {
            return false;
        }
    }

    return true;
}

std::string keyser::utils::localTimestamp()
{
    time_t t = time(NULL);
    struct tm *tmp = localtime(&t);

    std::string hourStr = std::to_string(tmp->tm_hour);
    std::string minStr  = std::to_string(tmp->tm_min);
    std::string secStr  = std::to_string(tmp->tm_sec);

    int hourStrLen = hourStr.size();
    int minStrLen  = minStr.size();
    int secStrLen  = secStr.size();

    std::string timestamp = "[" + 
                            std::string(2 - std::min(2, hourStrLen), '0') + hourStr + ":" +
                            std::string(2 - std::min(2, minStrLen), '0')  + minStr  + ":" +
                            std::string(2 - std::min(2, secStrLen), '0')  + secStr  + "] ";

    return timestamp;
}

std::string keyser::utils::localTime(time_t time)
{
    struct tm *tmp = localtime(&time);

    std::string hourStr = std::to_string(tmp->tm_hour);
    std::string minStr  = std::to_string(tmp->tm_min);
    std::string secStr  = std::to_string(tmp->tm_sec);

    int hourStrLen = hourStr.size();
    int minStrLen  = minStr.size();
    int secStrLen  = secStr.size();

    std::string localTime = std::string(2 - std::min(2, hourStrLen), '0') + hourStr + ":" +
                            std::string(2 - std::min(2, minStrLen), '0')  + minStr  + ":" +
                            std::string(2 - std::min(2, secStrLen), '0')  + secStr;

    return localTime;
}
