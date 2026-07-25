#pragma once

#include <string>

class DataStore;

class Persistence
{
public:
    static bool save(
        const DataStore& store,
        const std::string& filename);

    static bool load(
        DataStore& store,
        const std::string& filename);
};