#ifndef CHAIN_VIEW_H
#define CHAIN_VIEW_H

#include "../../node/Node.hpp"


namespace keyser
{
    namespace cli
    {
        class ChainView
        {
            public:
                ChainView(node::Node* node, bool& miningStatus);

                void display();

            private:
                node::Node* _node;

                bool& _miningStatus; 
        };
    }
}

#endif