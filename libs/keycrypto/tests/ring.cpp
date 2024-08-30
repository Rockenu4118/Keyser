#include <keycrypto/Stealth.hpp>
#include <keycrypto/Ring.hpp>


void test()
{
    crypto::StealthKeys aj;
    crypto::StealthKeys bob;

    std::string R;

    std::string fakeP1 = aj.genStealthAddr(bob.getPublicAddr(), R);
    std::string fakeP2 = aj.genStealthAddr(bob.getPublicAddr(), R);
    std::string fakeP3 = aj.genStealthAddr(bob.getPublicAddr(), R);

    std::string P = aj.genStealthAddr(bob.getPublicAddr(), R);
    std::string x = bob.genStealthKey(P, R);

    bool validAddr = bob.verifyStealthAddr(P, R);
    bool validKey = bob.verifyStealthKey(P, x);

    std::cout << "Bobs output: " << validAddr << std::endl;
    std::cout << "Valid Stealth Key: " << validKey << std::endl;

    std::string msg = crypto::hash::SHA3("dawg");
    std::string modmsg = crypto::hash::SHA3("dawgg");

    std::cout << "Legit one: " << P << std::endl;

    std::vector<std::string> keys;
    keys.push_back(fakeP1);
    keys.push_back(P);
    keys.push_back(fakeP2);
    // keys.push_back(fakeP3);

    // RNG gen and suffle key order within ring
    std::random_device rd;
    std::mt19937 g(rd());
  
    // Shuffle the vector 
    shuffle(keys.begin(), keys.end(), g);

    crypto::Ring ring(keys, P);
    // ring.printRing();
    std::vector<std::string> sig = ring.sign(msg, x);
    std::vector<std::string> fakesig = ring.sign(modmsg, x);

    // std::cout << "-----Sig-----\n"; 
    // for (auto elem : sig)
    //     std::cout << elem << "\n";
    // std::cout << "-----Sig-----\n";


    bool verified1 = crypto::Ring::verify(msg, sig, keys);
    bool verified2 = crypto::Ring::verify(modmsg, fakesig, keys);
    std::cout << "Sig 1 Verified: " << verified1 << "\n";
    std::cout << "Sig 2 Verified: " << verified2 << "\n";
}