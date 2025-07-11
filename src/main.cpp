//
//  Keyser Blockchain Protocol
//
//  Created by AJ Hermosillo on July 13, 2023.
//  Copyright (c) 2023 Hermosillo. All rights reserved.
//

#include "./app/App.hpp"

int main(int argc, char* argv[])
{
    auto* app = new keyser::App();
    app->init(argc, argv);

    return 0;
}