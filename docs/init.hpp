#ifndef INIT_H
#define INIT_H

#include <iostream>
#include <string>
#include <ctime>
#include <vector>
#include <unistd.h>

#include <boost/asio.hpp>
#include <boost/asio/ts/buffer.hpp>
#include <boost/asio/ts/internet.hpp>

class InitNet
{
    public:
        int initNet();

    private:
        void fetchData(boost::asio::ip::tcp::socket& socket);

    private:
        std::vector<char> _vBuffer;

};


#endif