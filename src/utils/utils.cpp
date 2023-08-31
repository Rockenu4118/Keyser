#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <cryptography.hpp>

#include "./utils.hpp"


std::string keyser::utils::pubKeytoAddress(const std::string& uPublicKey)
{   
    // Step 1: Convert the raw hex characters of the uncompressed public key to a string.
    std::string unhashed = hexToString(uPublicKey);
    std::string hashed = "";

    // Step 2: Generate a sha256 hash of the uncompressed public key.
    cryptography::sha256(unhashed, hashed);

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