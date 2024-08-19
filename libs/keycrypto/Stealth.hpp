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

            StealthKeys(std::string key);

            ~StealthKeys() = default;

            std::string getPublicAddr() const;

            std::string genStealthAddr(std::string recipient, std::string& RStr) const;

            std::string genStealthKey(std::string addr, std::string RStr) const;

            bool verifyStealthAddr(std::string addr, std::string RStr) const;

            std::string genImage(std::string addr, std::string RStr);

            inline std::string getPrivateSpendKey() const { return _ecKeys->getPrivateKey(); }
            inline std::string getPublicSpendKey()  const { return _ecKeys->getCPublicKey(); }
            inline std::string getPrivateViewKey()  const { return _privateViewKey; }
            inline std::string getPublicViewKey()   const { return _publicViewKey;  }

        private:
            void genViewKeys();

            std::shared_ptr<asym::ECKeyPair> _ecKeys;

            std::string _privateViewKey;
            std::string _publicViewKey;
    };
}


#endif