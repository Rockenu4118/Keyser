#ifndef OWNED_MESSAGE_H
#define OWNED_MESSAGE_H

#include <iostream>
#include <memory>

#include "./Peer.hpp"
#include "./NetMessage.hpp"

namespace keyser
{
    class Peer;

    struct OwnedMessage
    {
        OwnedMessage(std::shared_ptr<Peer> remote, NetMessage msg)
        {
            _remote = remote;
            _msg = msg;
        }

        std::shared_ptr<Peer> _remote = nullptr;
        NetMessage            _msg;
    };
}

#endif