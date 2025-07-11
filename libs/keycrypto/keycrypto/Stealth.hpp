#ifndef STEALTH_H
#define STEALTH_H

#include <iostream>
#include <string>
#include <memory>
#include <unistd.h>

#include "./ec.hpp"

namespace crypto
{
    class StealthKeys
    {
        friend std::ostream& operator<<(std::ostream& out, StealthKeys& data);

    public:
        // For generating a new keypair
        StealthKeys();

        // For restoring a keypair from a seed
        // StealthKeys()

        const BIGNUM* getPrivKey() const;
            //
            // explicit StealthKeys(std::string key);
            //
            // ~StealthKeys() = default;
            //
            // std::string getPublicAddr() const;
            //
            // static std::string genStealthAddr(std::string recipient, uint8_t index, std::string rStr);
            //
            // std::string deriveStealthKey(std::string addr, uint8_t index, std::string RStr) const;
            //
            // bool identifyOutput(std::string addr, uint8_t index, std::string RStr) const;
            //
            // bool verifyOutputOwnership(std::string addr, std::string key) const;
            //
            // std::string deriveImage(std::string addr, uint8_t index, std::string RStr);
            //
            // inline std::string getPrivateSpendKey() const { return _ecKeys->getPrivateKey(); }
            // inline std::string getPublicSpendKey()  const { return _ecKeys->getCPublicKey(); }
            // inline std::string getPrivateViewKey()  const { return _privateViewKey; }
            // inline std::string getPublicViewKey()   const { return _publicViewKey;  }
            //
            // static std::string genTxPrivKey();
            //
            // static std::string deriveTxPubKey(std::string rStr);

    private:
        bool genKeyPairObj();

        EVP_PKEY* mKeyPairObj = nullptr;
            // void deriveViewKeys();

        // std::shared_ptr<ECKeyPair> _ecKeys;

            // std::string _privateViewKey;
            // std::string _publicViewKey;
    };
}


#endif