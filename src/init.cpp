#include <iostream>
#include <string>
#include <ctime>

#include <boost/asio.hpp>
#include <boost/asio/ts/buffer.hpp>
#include <boost/asio/ts/internet.hpp>

#include "./include/init.hpp"
#include "./include/Chain.hpp"
#include "./include/Block.hpp"
#include "./include/Wallet.hpp"


int InitChain::initChain()
{
    /*************************************************
        Initialize wallets and print contents 
    *************************************************/
    std::string key1 = "38B15E3C2210827DEB42C39FBC04D2D8268B5F7B7F1DC2DA75FD620BBD2F4E01";
    std::string key2 = "18E14A7B6A307F426A94F8114701E7C8E774E7F9A47E2C2035DB29A206321725";

    Keyser::Wallet ajWallet = Keyser::Wallet("AJ", key1);
    std::cout << ajWallet << std::endl;

    Keyser::Wallet guyWallet = Keyser::Wallet("Guy", key2);
    std::cout << guyWallet << std::endl;

    /*************************************************
        Initialize chain
    *************************************************/
    std::cout << "\nInitializing chain...\n" << std::endl;
    Keyser::Chain chain = Keyser::Chain(1, 100);
    std::cout << *chain.getCurrBlock() << std::endl;

    Keyser::Transaction tx = Keyser::Transaction(100, "0xc6d8a2c830495d07318212e9f2cad16f", guyWallet.getKeyPair()->getUPublicKey());
    tx.sign(guyWallet.getKeyPair());
    tx.isValid();
    chain.createTransaction(tx);

    chain.mineBlock("0xc6d8a2c830495d07318212e9f2cad16f");
    // std::cout << *chain.getCurrBlock() << std::endl;

    chain.mineBlock("0xc6d8a2c830495d07318212e9f2cad16f");
    // std::cout << *chain.getCurrBlock() << std::endl;


    chain.getAddressBalance("0xc6d8a2c830495d07318212e9f2cad16f");

    return 0;
}

int InitNet::initNet()
{
    boost::system::error_code ec;

    boost::asio::io_context context;

    boost::asio::io_context::work idleWork(context);

    std::thread thrContext = std::thread([&]() { context.run(); });

    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::make_address("104.18.115.97", ec), 80);

    boost::asio::ip::tcp::socket socket(context);

    socket.connect(endpoint, ec);

    if (!ec) {
        std::cout << "Connected" << std::endl;
    } else {
        std::cout << "Failed to connect:\n" << ec.message() << std::endl;
    }

    if (socket.is_open()) {
        std::cout << "Socket is open" << std::endl;

        _vBuffer.resize(256);
        fetchData(socket);

        std::string request = 
            "GET / HTTP/1.1\r\n"
            "Host: icanhazip.com\r\n"
            "Connection: close\r\n\r\n";

        socket.write_some(boost::asio::buffer(request.data(), request.size()), ec);
        

        sleep(1);

        context.stop();
        
        if (thrContext.joinable()) {
            thrContext.join();
        }
    }


    return 0;
}

void InitNet::fetchData(boost::asio::ip::tcp::socket& socket)
{
    socket.async_read_some(boost::asio::buffer(_vBuffer.data(), _vBuffer.size()),
        [&](std::error_code ec, std::size_t length)
        {
            if (!ec)
            {
                std::cout << "\n\nRead " << length << " bytes\n\n";

                for (int i = 0 ; i < length ; i++) {
                    std::cout << _vBuffer[i];
                }

                fetchData(socket);
            }
        }
    );
}