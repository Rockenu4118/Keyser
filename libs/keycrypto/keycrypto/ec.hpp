#ifndef EC_H
#define EC_H

#include <iostream>
#include <string>
#include <openssl/ec.h>
#include <openssl/evp.h>
#include <nlohmann/json.hpp>

namespace crypto
{
    enum class KeyType : uint8_t
    {
        Private,
        Public
    };

    class ECKeyPair
    {
        // IO Stream operators
        // friend std::ostream& operator<<(std::ostream& out, ECKeyPair& data);

    public:
        ECKeyPair();
        // ECKeyPair(nlohmann::json json);
        // ECKeyPair(KeyType type, std::string privateKey);

        EVP_PKEY*     getKeyPairObj() { return mKeyPairObj; };
        // inline std::string getPrivateKey() { return _privateKey; };
        // inline std::string getUPublicKey() { return _uPublicKey; };
        // inline std::string getCPublicKey() { return _cPublicKey; };
        //
        bool genKeyPairObj();
        // bool insertPrivateKey(std::string privateKey);
        // bool insertPublicKey(std::string publicKey);
        // bool extractKeys();
        //
        // nlohmann::json json() const;
        //
        // void sign(std::string hash, std::string& rSigVal, std::string& sSigVal);
        // bool verify(std::string hash, std::string rSigVal, std::string sSigVal);

    private:
        // EC_KEY*     _keyPairObj;
        EVP_PKEY* mKeyPairObj = nullptr;
        // std::string _privateKey;
        // std::string _uPublicKey;
        // std::string _cPublicKey;
    };
}


#endif