#include <mutex>
#include <condition_variable>
#include <deque>


#include "./tsqueue.hpp"

// Destructor
template <typename T>
networking::tsqueue<T>::~tsqueue()
{
    clear();
}

// Accessors
template <typename T>
const T& networking::tsqueue<T>::front()
{
    std::scoped_lock lock(muxQueue);
    return deqQueue.front();
}

template <typename T>
const T& networking::tsqueue<T>::back()
{
    std::scoped_lock lock(muxQueue);
    return deqQueue.back();
}

// Modifiers
template <typename T>
void networking::tsqueue<T>::pushFront(const T& item)
{
    std::scoped_lock lock(muxQueue);
    deqQueue.emplace_front(std::move(item));
}

template <typename T>
void networking::tsqueue<T>::pushBack(const T& item)
{
    std::scoped_lock lock(muxQueue);
    deqQueue.emplace_back(std::move(item));
}

template <typename T>
bool networking::tsqueue<T>::empty()
{
    std::scoped_lock lock(muxQueue);
    return deqQueue.empty();
}

template <typename T>
void networking::tsqueue<T>::clear()
{
    std::scoped_lock lock(muxQueue);
    deqQueue.clear();
}

template <typename T>
size_t networking::tsqueue<T>::count()
{
    std::scoped_lock lock(muxQueue);
    return deqQueue.size();
}