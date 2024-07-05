#ifndef TSQUEUE_H
#define TSQUEUE_H

#include <mutex>
#include <condition_variable>
#include <deque>

namespace keyser
{
    template<typename T>
    class tsqueue
    {
        public:
            // Constructors and Destructor
            tsqueue() = default;
            tsqueue(const tsqueue<T>&) = delete;

            virtual ~tsqueue()
            {
                clear();
            }

            const T& front()
            {
                std::scoped_lock lock(muxQueue);
                return deqQueue.front();
            }

            const T& back()
            {
                std::scoped_lock lock(muxQueue);
                return deqQueue.back();
            }

            T popFront()
            {
                std::scoped_lock lock(muxQueue);
                auto t = std::move(deqQueue.front());
                deqQueue.pop_front();
                return t;
            }

            T popBack()
            {
                std::scoped_lock lock(muxQueue);
                auto t = std::move(deqQueue.back());
                deqQueue.pop_back();
                return t;
            }

            void pushBack(const T& item)
            {
                std::scoped_lock lock(muxQueue);
                deqQueue.emplace_back(std::move(item));
            }

            void pushFront(const T& item)
            {
                std::scoped_lock lock(muxQueue);
                deqQueue.emplace_front(std::move(item));
            }

            const T& at(int index)
            {
                std::scoped_lock lock(muxQueue);
                return deqQueue.at(index);
            }

            bool empty()
            {
                std::scoped_lock lock(muxQueue);
                return deqQueue.empty();
            }

            void clear()
            {
                std::scoped_lock lock(muxQueue);
                deqQueue.clear();
            }

            size_t count()
            {
                std::scoped_lock lock(muxQueue);
                return deqQueue.size();
            }
            
            
        protected:
            std::mutex    muxQueue;
            std::deque<T> deqQueue;
    };
}

#endif