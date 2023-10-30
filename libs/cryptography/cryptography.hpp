#ifndef CRYPTOGRAPHY_H
#define CRYPTOGRAPHY_H

#include <string>
#include <openssl/ec.h>


namespace cryptography
{
    // Hashing
    bool SHA256(const std::string& unhashed, std::string& hashed);    

    // Assymmetric encryption
    class ECKeyPair
    {
        // IO Stream operators
        friend std::ostream& operator<<(std::ostream& out, ECKeyPair& data);

        public:
            // Constructors
            ECKeyPair();
            ECKeyPair(std::string keyType, std::string privateKey);

            // Accessors
            EC_KEY*     getKeyPairObj();
            std::string getPrivateKey();
            std::string getUPublicKey();
            std::string getCPublicKey();

            // Modifiers
            bool genKeyPairObj();
            bool insertPrivateKey(std::string privateKey);
            bool insertPublicKey(std::string publicKey);
            bool extractKeys();

            // Other
            void sign(std::string hash, std::string& rSigVal, std::string& sSigVal);
            bool verify(std::string hash, std::string rSigVal, std::string sSigVal);

        private:
            EC_KEY*     _keyPairObj;
            std::string _privateKey;
            std::string _uPublicKey;
            std::string _cPublicKey;
    }; 
}

#endif