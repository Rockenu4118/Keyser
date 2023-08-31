//
//  Keyser Blockchain Protocol
//
//  Created by AJ Hermosillo on July 13, 2023.
//  Copyright (c) 2015 2023 Hermosillo. All rights reserved.
//

#include <iostream>
#include <string>
#include <ctime>
#include <vector>
#include <unistd.h>

#include <boost/asio.hpp>
#include <boost/asio/ts/buffer.hpp>
#include <boost/asio/ts/internet.hpp>

#include "./include/init.hpp"

// std::vector<char> vBuffer(256);

// void fetchData(boost::asio::ip::tcp::socket& socket)
// {
//     socket.async_read_some(boost::asio::buffer(vBuffer.data(), vBuffer.size()),
//         [&](std::error_code ec, std::size_t length)
//         {
//             if (!ec)
//             {
//                 std::cout << "\n\nRead " << length << " bytes\n\n";

//                 for (int i = 0 ; i < length ; i++) {
//                     std::cout << vBuffer[i];
//                 }

//                 fetchData(socket);
//             }
//         }
//     );
// }

int main()
{
    // Begin chain initialization and sequence
    initChain();

    // TCP Stuff

    // boost::system::error_code ec;

    // boost::asio::io_context context;

    // boost::asio::io_context::work idleWork(context);

    // std::thread thrContext = std::thread([&]() { context.run(); });

    // boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::make_address("104.18.115.97", ec), 80);

    // boost::asio::ip::tcp::socket socket(context);

    // socket.connect(endpoint, ec);

    // if (!ec) {
    //     std::cout << "Connected" << std::endl;
    // } else {
    //     std::cout << "Failed to connect:\n" << ec.message() << std::endl;
    // }

    // if (socket.is_open()) {
    //     std::cout << "Socket is open" << std::endl;

    //     fetchData(socket);

    //     std::string request = 
    //         "GET / HTTP/1.1\r\n"
    //         "Host: icanhazip.com\r\n"
    //         "Connection: close\r\n\r\n";

    //     socket.write_some(boost::asio::buffer(request.data(), request.size()), ec);
        

    //     sleep(1);

    //     context.stop();
        
    //     if (thrContext.joinable()) {
    //         thrContext.join();
    //     }
    // }

    // std::cout << "hello" << std::endl;

    return 0;
}