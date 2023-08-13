#ifndef ECKEYPAIR_H
#define ECKEYPAIR_H

#include <string>
#include <openssl/ec.h>

namespace Keyser
{
    class ECKeyPair
    {
        // IO Stream operators
        friend std::ostream& operator<<(std::ostream& out, ECKeyPair& data);

        public:
            // Constructors
            ECKeyPair();
            ECKeyPair(std::string privateKey);

            // Accessors
            EC_KEY*     getKeyPairObj();
            std::string getPrivateKey();
            std::string getUPublicKey();
            std::string getCPublicKey();

            // Modifiers
            bool genKeyPairObj();
            bool insertPrivateKey(std::string privateKey);
            bool extractKeys();

        private:
            EC_KEY*     _keyPairObj;
            std::string _privateKey;
            std::string _uPublicKey;
            std::string _cPublicKey;
    };
}

#endif