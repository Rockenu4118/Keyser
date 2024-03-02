#ifndef NETWORK_VIEW_H
#define NETWORK_VIEW_H

#include <iostream>
#include <CLI_View.hpp>

#include "../node/Node.hpp"


namespace keyser
{
    namespace cli
    {
        class NetworkView : public cli_core::CLI_View
        {
            public:
                NetworkView(Node* node);

            protected:
                void display();

                void newConnection();

            private:
                Node* _node = nullptr;
        };
    }
}

#endif