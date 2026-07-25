#include "DataStore.h"

void DataStore::set(const std::string& key,
    const std::string& value)
{
std::lock_guard<std::mutex> lock(mutex_);

Entry entry;
entry.value = value;
entry.hasExpiry = false;

database_[key] = entry;
}

void DataStore::set(const std::string& key,
    const std::string& value,
    int ttlSeconds)
{
std::lock_guard<std::mutex> lock(mutex_);

Entry entry;
entry.value = value;
entry.hasExpiry = true;
entry.expiresAt =
std::chrono::steady_clock::now() +
std::chrono::seconds(ttlSeconds);

database_[key] = entry;
}

std::string DataStore::get(const std::string& key)
{
    std::lock_guard<std::mutex> lock(mutex_);

    auto it = database_.find(key);

    if(it == database_.end())
        {return "(nil)";}
    if (it->second.hasExpiry &&
            std::chrono::steady_clock::now() >
            it->second.expiresAt)
        {
            database_.erase(it);
        
            return "(nil)";
        }

    return it->second.value;
}

bool DataStore::del(const std::string& key)
{
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = database_.find(key);
    if (it == database_.end())
    {
        return false;
    }
    database_.erase(it);
    return true;
}
std::vector<std::string> DataStore::keys()
{
    std::lock_guard<std::mutex> lock(mutex_);
    std::vector<std::string> result;
    for (const auto& pair : database_)
    {
        result.push_back(pair.first);
    }

    return result;
}
const std::unordered_map<std::string, Entry>&
DataStore::getDatabase() const
{
    return database_;
}
void DataStore::removeExpiredKeys()
{
    std::lock_guard<std::mutex> lock(mutex_);
    auto now = std::chrono::steady_clock::now();
    for (auto it = database_.begin(); it != database_.end();)
    {
        if (it->second.hasExpiry &&
            now > it->second.expiresAt)
        {
            it = database_.erase(it);
        }
        else
        {
            ++it;
        }
    }
}
size_t DataStore::size() const
{
    std::lock_guard<std::mutex> lock(mutex_);
    return database_.size();
}