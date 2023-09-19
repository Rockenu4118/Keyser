#include <iostream>
#include <memory>
#include <cstring>

#include "./net_message.hpp"


template <typename T>
size_t networking::Message<T>::size() const
{
    return body.size();
}
