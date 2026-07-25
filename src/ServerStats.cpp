#include "ServerStats.h"

ServerStats::ServerStats()
{
    clients_ = 0;

    startTime_ = std::chrono::steady_clock::now();
}

void ServerStats::clientConnected()
{
    clients_++;
}

void ServerStats::clientDisconnected()
{
    clients_--;
}

int ServerStats::clientCount() const
{
    return clients_;
}

long long ServerStats::uptimeSeconds() const
{
    auto now = std::chrono::steady_clock::now();

    return std::chrono::duration_cast<
        std::chrono::seconds>(
            now - startTime_
        ).count();
}