#include "Persistence.h"
#include "DataStore.h"

#include <fstream>

bool Persistence::save(
    const DataStore& store,
    const std::string& filename)
{
    std::ofstream file(filename);

    if (!file.is_open())
    {
        return false;
    }

    const auto& database = store.getDatabase();

    for (const auto& pair : database)
    {
        file << pair.first
     << "="
     << pair.second.value
     << "\n";
    }

    file.close();

    return true;
}
#include <sstream>

bool Persistence::load(
    DataStore& store,
    const std::string& filename)
{
    std::ifstream file(filename);

    if (!file.is_open())
    {
        return false;
    }

    std::string line;

    while (std::getline(file, line))
    {
        std::stringstream ss(line);

        std::string key;
        std::string value;

        if (std::getline(ss, key, '=') &&
            std::getline(ss, value))
        {
            store.set(key, value);
        }
    }

    file.close();

    return true;
}