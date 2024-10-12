#include "../keycrypto/Stealth.hpp"

void stealth_test()
{
    crypto::StealthKeys aj;
    crypto::StealthKeys bob;

    std::string r = crypto::StealthKeys::genTxPrivKey();
    std::string R = crypto::StealthKeys::deriveTxPubKey(r);


    std::cout << "r: " << r << "\n" << "R: " << R << "\n";

    std::string Output0 = aj.genStealthAddr(bob.getPublicAddr(), 0, r);

    std::cout << R << "\n";
    //
    std::string Output1 = aj.genStealthAddr(bob.getPublicAddr(), 1, r);
    std::cout << R << "\n";
    //
    std::string Output2 = aj.genStealthAddr(bob.getPublicAddr(), 2, r);

    std::cout << R << "\n";

    std::cout << "Output0: " << Output0 << "\n" << "Output1: " << Output1 << "\n" << "Output2: " << Output2 << "\n";

    std::cout << "----------Verify ownership---------\n";

    std::cout << "Output0: " << bob.identifyOutput(Output0, 0, R) << "\n";
    std::cout << "Output1: " << bob.identifyOutput(Output1, 1, R) << "\n";
    std::cout << "Output2: " << bob.identifyOutput(Output2, 2, R) << "\n";

    std::cout << "----------Derive x and verify---------\n";

    std::string x0 = bob.deriveStealthKey(Output0, 0, R);
    std::string x1 = bob.deriveStealthKey(Output1, 1, R);
    std::string x2 = bob.deriveStealthKey(Output2, 2, R);

    std::cout << "x0: " << x0 << "\n" << "x1: " << x1 << "\n" << "x2: " << x2 << "\n";

    std::cout << "x0: " << bob.verifyOutputOwnership(Output0, x0) << "\n";
    std::cout << "x1: " << bob.verifyOutputOwnership(Output1, x1) << "\n";
    std::cout << "x2: " << bob.verifyOutputOwnership(Output2, x2) << "\n";


}