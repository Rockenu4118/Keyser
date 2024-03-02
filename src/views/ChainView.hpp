#ifndef CHAIN_VIEW_H
#define CHAIN_VIEW_H

#include <CLI_View.hpp>

#include "../node/Node.hpp"


namespace keyser
{
    namespace cli
    {
        class ChainView : public cli_core::CLI_View
        {
            public:
                ChainView(Node* node);

                void viewBlockTransactions();

            protected:
                void display();

            private:
                Node* _node = nullptr;
        };
    }
}

#endif