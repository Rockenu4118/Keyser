#include "../keycrypto/Stealth.hpp"
#include "../keycrypto/Ring.hpp"
#include "../keycrypto/utils.hpp"
#include <time.h>
#include <memory>


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

    std::vector<std::string> keys;
    keys.push_back(fakeP1);
    keys.push_back(P);
    keys.push_back(fakeP2);

    // BIGNUM* bruh = nullptr;
    // std::unique_ptr<BIGNUM> ptr = std::make_unique<BIGNUM>(BN_new());
    // bruh = ptr.get();

    // crypto::utils::print(bruh);



    // RNG gen and suffle key order within ring
    std::random_device rd;
    std::mt19937 g(rd());
  
    // Shuffle the vector
    shuffle(keys.begin(), keys.end(), g);

    // ring.printRing();
    std::string image = bob.genImage(P, R);
    std::cout << "Image: " << image << std::endl;
    double time1, timedif;

  

    std::vector<std::string> sig = crypto::Ring::sign(msg, keys, P, x, image);
    std::vector<std::string> fakesig = crypto::Ring::sign(modmsg, keys, P, x, image);

    // crypto::Ring::printSig(sig);

    // time1 = (double) clock();            /* get initial time */
    // time1 = time1 / CLOCKS_PER_SEC;      /*    in seconds    */

    bool verified1 = crypto::Ring::verify(msg, sig, keys, image);

    // timedif = ( ((double) clock()) / CLOCKS_PER_SEC) - time1;
    // printf("The elapsed time is %lf seconds\n", timedif);

    bool verified2 = crypto::Ring::verify(modmsg, fakesig, keys, image);
    std::cout << "Sig 1 Verified: " << verified1 << "\n";
    std::cout << "Sig 2 Verified: " << verified2 << "\n";



    

    

    
}