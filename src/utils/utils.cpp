#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <ctime>
// #include <algorithm>

#include "./utils.hpp"
#include "../wallet/Wallet.hpp"
#include "../primitives/Block.hpp"
#include "../node/PeerInfo.hpp"

//
// std::string keyser::utils::pubKeytoAddress(const std::string& uPublicKey)
// {
//     // Step 1: Convert the raw hex characters of the uncompressed public key to a string.
//     std::string unhashed = hexToString(uPublicKey);
//     std::string hashed = "";
//
//     // Step 2: Generate a sha256 hash of the uncompressed public key.
//     crypto::hash::SHA256(unhashed, hashed);
//
//     // Step 3: Extract last 32 (hex) characters, 16 bytes, of the sha256 hash.
//     std::string address = hashed.substr(32, 32);
//
//     // Step 4: Prepend "0x" to indicate that the address is in hex format.
//     std::string publicAddress = "0x" + address;
//
//     return publicAddress;
// }

std::vector<std::string> keyser::utils::parse_cmd(const std::string input)
{
    std::vector<std::string> args;
    std::string current;
    bool in_quotes = false;
    char quote_char = '\0';

    for (size_t i = 0; i < input.size(); ++i) {
        char c = input[i];

        if (in_quotes) {
            if (c == quote_char) {
                in_quotes = false;
            } else if (c == '\\' && i + 1 < input.size()) {
                // Handle escaped characters
                current += input[++i];
            } else {
                current += c;
            }
        } else {
            if (std::isspace(c)) {
                if (!current.empty()) {
                    args.push_back(current);
                    current.clear();
                }
            } else if (c == '"' || c == '\'') {
                in_quotes = true;
                quote_char = c;
            } else {
                current += c;
            }
        }
    }

    if (!current.empty()) {
        args.push_back(current);
    }

    return args;
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

std::string keyser::utils::localTimestamp()
{
    time_t t = time(NULL);
    tm *tmp = localtime(&t);

    std::string hourStr = std::to_string(tmp->tm_hour);
    std::string minStr  = std::to_string(tmp->tm_min);
    std::string secStr  = std::to_string(tmp->tm_sec);

    int hourStrLen = hourStr.size();
    int minStrLen  = minStr.size();
    int secStrLen  = secStr.size();

    std::string timestamp = "[" + 
                            std::string(2 - std::min(2, hourStrLen), '0') + hourStr + ":" +
                            std::string(2 - std::min(2, minStrLen), '0')  + minStr  + ":" +
                            std::string(2 - std::min(2, secStrLen), '0')  + secStr  + "]";

    return timestamp;
}

std::string keyser::utils::localTime(time_t time)
{
    const tm *tmp = localtime(&time);

    std::string hourStr = std::to_string(tmp->tm_hour);
    std::string minStr  = std::to_string(tmp->tm_min);
    std::string secStr  = std::to_string(tmp->tm_sec);

    int hourStrLen = hourStr.size();
    int minStrLen  = minStr.size();
    int secStrLen  = secStr.size();

    std::string localTime = std::string(2 - std::min(2, hourStrLen), '0') + hourStr + ":" +
                            std::string(2 - std::min(2, minStrLen), '0')  + minStr  + ":" +
                            std::string(2 - std::min(2, secStrLen), '0')  + secStr;

    delete tmp;

    return localTime;
}
