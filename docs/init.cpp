#include <iostream>
#include <string>
#include <ctime>

#include <boost/asio.hpp>
#include <boost/asio/ts/buffer.hpp>
#include <boost/asio/ts/internet.hpp>


void fetchData(boost::asio::ip::tcp::socket& socket, std::vector<char>& vBuffer)
{
    socket.async_read_some(boost::asio::buffer(vBuffer.data(), vBuffer.size()),
        [&](std::error_code ec, std::size_t length)
        {
            if (!ec)
            {
                std::cout << "\n\nRead " << length << " bytes\n\n";

                for (int i = 0 ; i < length ; i++) {
                    std::cout << vBuffer[i];
                }

                fetchData(socket, vBuffer);
            }
        }
    );
}

int initNet()
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

        std::vector<char> vBuffer;
        vBuffer.resize(1024);
        // fetchData(socket, vBuffer);
        boost::beast::flat_buffer buffer;
        boost::beast::http::response<boost::beast::http::string_body> response;
        

        

        std::string request = 
            "GET / HTTP/1.1\r\n"
            "Host: icanhazip.com\r\n"
            "Connection: close\r\n\r\n";

        socket.write_some(boost::asio::buffer(request.data(), request.size()), ec);
        

        sleep(1);

        boost::beast::http::read(socket, buffer, response);

        std::string ip = response.body();

        std::cout << ip << std::endl;    

        context.stop();
        
        if (thrContext.joinable()) {
            thrContext.join();
        }
    }


    return 0;
}