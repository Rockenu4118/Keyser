#ifndef OWNED_MESSAGE_H
#define OWNED_MESSAGE_H

#include <iostream>
#include <memory>

#include "./Peer.hpp"
#include "./Message.hpp"

namespace keyser
{
    class Peer;

    struct OwnedMessage
    {
        friend std::ostream& operator<<(std::ostream& out, const OwnedMessage& msg) {
            out << msg._msg;
            return out;
        }

        OwnedMessage(std::shared_ptr<Peer> remote, Message msg)
        {
            _remote = remote;
            _msg    = msg;
        }

        std::shared_ptr<Peer> _remote = nullptr;
        Message               _msg;
    };
}

#endif