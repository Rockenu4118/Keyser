#ifndef STORAGE_ENGINE_H
#define STORAGE_ENGINE_H


namespace keyser
{
    class StorageEngine
    {
        public:
            StorageEngine() = default;

            int  chainHeight() const;
            void loadChain();


        private:

    };
}

#endif