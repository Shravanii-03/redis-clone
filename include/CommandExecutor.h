#pragma once

#include <string>
#include <vector>

#include "ClientSession.h"
#include "DataStore.h"
#include "ServerStats.h"

class CommandExecutor
{
public:
    // session and password are optional. If password is empty the server
    // runs without authentication (the original behaviour).
    CommandExecutor(DataStore& store,
                    ServerStats& stats,
                    ClientSession* session = nullptr,
                    std::string password = "");

    std::string execute(const std::vector<std::string>& tokens);

private:
    DataStore& store_;
    ServerStats& stats_;
    ClientSession* session_;
    std::string password_;
};