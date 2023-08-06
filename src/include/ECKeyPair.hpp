#ifndef ECKEYPAIR_H
#define ECKEYPAIR_H

#include <string>
#include <openssl/ec.h>

namespace Keyser
{
    class ECKeyPair
    {
        public:
            // Constructors
            ECKeyPair();

            // Accessors
            std::string getPrivateKey();
            std::string getUPublicKey();
            std::string getCPublicKey();

            // Modifiers
            bool genKeyPairObj();
            bool extractKeys();

            // Other
            void printKeys();

        private:
            EC_KEY*     _keyPairObj;
            std::string _privateKey;
            std::string _uPublicKey;
            std::string _cPublicKey;
    };
}

#endif