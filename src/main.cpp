#include "Server.h"

#include <iostream>

int main()
{
    constexpr int kDefaultPort = 6379;

    Server server(kDefaultPort);

    if (!server.start())
    {
        std::cerr << "Failed to start MiniRedis server.\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}