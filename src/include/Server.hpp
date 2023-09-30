#include <Server_Interface.hpp>

enum class CustomMsgTypes : uint32_t
{
    ServerAccept,
    ServerDeny,
    ServerPing
};

class Server : public networking::Server_Interface<CustomMsgTypes>
{
    public:
        Server(uint16_t port) : networking::Server_Interface<CustomMsgTypes>(port)
        {

        }

    protected: 
        virtual bool onClientConnect(std::shared_ptr<networking::Connection<CustomMsgTypes>> client)
        {
            return true;
        }

        virtual void onMessage(std::shared_ptr<networking::Connection<CustomMsgTypes>> client, networking::Message<CustomMsgTypes>& msg)
        {}
};