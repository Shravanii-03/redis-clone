#include "Server.h"

#include <cstdlib>
#include <iostream>
#include <string>

int main()
{
    constexpr int kDefaultPort = 6379;

    // Optional password: set MINIREDIS_PASSWORD before starting the server.
    // If it is not set, authentication is disabled.
    std::string password;
    if (const char* env = std::getenv("MINIREDIS_PASSWORD"))
        password = env;

    Server server(kDefaultPort, password);

    if (!server.start())
    {
        std::cerr << "Failed to start MiniRedis server.\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}