#include "./connection.hpp"


template <typename T>
bool networking::Connection<T>::connectToServer()
{

}

template <typename T>
bool networking::Connection<T>::disconnect()
{

}

template <typename T>
bool networking::Connection<T>::isConnected() const
{
    return _socket.is_open();
}