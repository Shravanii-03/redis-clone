#pragma once

#include <chrono>
#include <string>

class ServerStats
{
public:
    ServerStats();
    void clientConnected();
    void clientDisconnected();
    int clientCount() const;
    long long uptimeSeconds() const;

private:
    int clients_;
    std::chrono::steady_clock::time_point startTime_;
};