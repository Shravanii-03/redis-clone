#pragma once

#include <winsock2.h>

#include <string>

#include "DataStore.h"
#include "ServerStats.h"

class Server
{
public:
    explicit Server(int port, std::string password = "");

    bool start();

private:
    // Configuration
    int port_;
    std::string password_;   // empty = authentication disabled

    // Networking
    SOCKET serverSocket_;

    // Database
    DataStore store_;

    // Runtime statistics
    ServerStats stats_;

    // Initialization
    bool initializeWinsock();
    bool createServerSocket();
    bool bindSocket();
    bool startListening();

    // Client handling
    void acceptClients();
    void handleClient(SOCKET clientSocket);
};