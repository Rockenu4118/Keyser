#ifndef STEALTH_H
#define STEALTH_H

#include <iostream>
#include <string>
#include <memory>
#include <unistd.h>

#include "./ec.hpp"
#include "./sha.hpp"

namespace crypto
{
    class StealthKeys
    {
        friend std::ostream& operator<<(std::ostream& out, StealthKeys& data);

        public:
            StealthKeys();

            explicit StealthKeys(std::string key);

            ~StealthKeys() = default;

            std::string getPublicAddr() const;

            static std::string genStealthAddr(std::string recipient, uint8_t index, std::string rStr);

            std::string deriveStealthKey(std::string addr, uint8_t index, std::string RStr) const;

            bool identifyOutput(std::string addr, uint8_t index, std::string RStr) const;

            bool verifyOutputOwnership(std::string addr, std::string key) const;

            std::string deriveImage(std::string addr, uint8_t index, std::string RStr);

            inline std::string getPrivateSpendKey() const { return _ecKeys->getPrivateKey(); }
            inline std::string getPublicSpendKey()  const { return _ecKeys->getCPublicKey(); }
            inline std::string getPrivateViewKey()  const { return _privateViewKey; }
            inline std::string getPublicViewKey()   const { return _publicViewKey;  }

            static std::string genTxPrivKey();

            static std::string deriveTxPubKey(std::string rStr);

        private:
            void deriveViewKeys();

            std::shared_ptr<asym::ECKeyPair> _ecKeys;

            std::string _privateViewKey;
            std::string _publicViewKey;
    };
}


#endif