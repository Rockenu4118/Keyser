#include <iostream>

#include <boost/beast.hpp>
#include <boost/asio.hpp>

using tcp = boost::asio::ip::tcp;


void handleRequest(boost::beast::http::request<boost::beast::http::string_body>& request, tcp::socket& socket) {
    // Prepare the response message
    boost::beast::http::response<boost::beast::http::string_body> response;
    response.version(request.version());
    response.result(boost::beast::http::status::ok);
    response.set(boost::beast::http::field::server, "My HTTP Server");
    response.set(boost::beast::http::field::content_type, "text/plain");
    response.body() = "Hello, World!";
    response.prepare_payload();

    // Send the response to the client
    boost::beast::http::write(socket, response);
}

void runServer() {
    boost::asio::io_context io_context;
    tcp::acceptor acceptor(io_context, {tcp::v4(), 8080});

    while (true) {
        tcp::socket socket(io_context);
        acceptor.accept(socket);

        std::cout << "Req" << std::endl;

        // Read the HTTP request
        boost::beast::flat_buffer buffer;
        boost::beast::http::request<boost::beast::http::string_body> request;
        boost::beast::http::read(socket, buffer, request);

        // Handle the request
        handleRequest(request, socket);

        // Close the socket
        socket.shutdown(tcp::socket::shutdown_send);
    }
}

int main()
{
    try {
        runServer();
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}