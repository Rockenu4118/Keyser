#ifndef TSQUEUE_H
#define TSQUEUE_H

#include <mutex>
#include <condition_variable>
#include <deque>

namespace networking
{
    template<typename T>
    class tsqueue
    {
        public:
            // Constructors and Destructor
            tsqueue() = default;
            tsqueue(const tsqueue<T>&) = delete;
            virtual ~tsqueue();

            // Accessors
            const T& front();
            const T& back();

            // Modifiers
            void pushBack(const T& item);
            void pushFront(const T& item);

            // Other
            bool   empty();
            void   clear();
            size_t count();

            // Other
            T popFront();
            T popBack();
            
            
        protected:
            std::mutex    muxQueue;
            std::deque<T> deqQueue;
    };
}

#endif