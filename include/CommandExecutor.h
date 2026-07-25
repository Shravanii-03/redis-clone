#pragma once

#include <string>
#include <vector>

#include "DataStore.h"
#include "ServerStats.h"

class CommandExecutor
{
public:
    CommandExecutor(DataStore& store,ServerStats& stats);
    std::string execute(const std::vector<std::string>& tokens);

private:
    DataStore& store_;
    ServerStats& stats_;
};