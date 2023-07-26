#include <iostream>
#include <string>
#include <ctime>

#include "./include/Chain.hpp"
#include "./include/Transaction.hpp"
#include "./include/Wallet.hpp"
#include "./include/util.hpp"

int main()
{
    Keyser::Chain chain = Keyser::Chain(4, 100);
    std::cout << "Chain initialized." << std::endl;

    chain.createGenesisBlock();

    chain.createTransaction(Keyser::Transaction(100, "aj", "him"));

    chain.mineBlock("theguy");

    chain.createTransaction(Keyser::Transaction(100, "aj", "him"));

    chain.mineBlock("theguy");

    chain.createTransaction(Keyser::Transaction(50, "him", "aj"));

    chain.mineBlock("theguy");

    chain.getAddressBalance("theguy");

    return 0;
}