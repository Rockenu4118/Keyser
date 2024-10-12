#ifndef CRYPTO_H
#define CRYPTO_H

#include <string>
#include <openssl/ec.h>
#include <nlohmann/json.hpp>

#include "./cryptoUtils.hpp"


namespace crypto
{
    enum class KeyType : uint8_t
    {
        Private,
        Public
    };

    // class
    // class ED25519KeyPair
    // {
    //     friend std::ostream& operator<<(std::ostream& out, ED25519KeyPair& data);

    //     public:
    //         ED25519KeyPair();

    //         ED25519KeyPair(KeyType type, std::string privateKey);

    //         bool genKeyPairObj();
    //         bool extractKeyPair();

    //         bool insertPrivateKey(std::string privateKey);
    //         bool insertPublicKey(std::string publicKey);

    //         std::string getPrivateKey() const { return _privateKey; }
    //         std::string getPublicKey()  const { return _publicKey;  }

    //     private:
    //         EVP_PKEY* _keyPairObj = nullptr;

    //         std::string _privateKey;
    //         std::string _publicKey;
    // };

    // Assymmetric encryption
    
}

#endif