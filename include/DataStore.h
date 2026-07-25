#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <mutex>
#include <chrono>

struct Entry
{
    std::string value;

    std::chrono::steady_clock::time_point expiresAt;

    bool hasExpiry = false;
};

class DataStore
{
public:
void set(const std::string& key,
    const std::string& value);
    size_t size() const;
    void set(const std::string& key, const std::string& value,int ttlSeconds);
    std::string get(const std::string& key);
    bool del(const std::string& key);
    std::vector<std::string> keys();
    const std::unordered_map<std::string, Entry>&getDatabase() const;

void removeExpiredKeys();
private:
std::unordered_map<std::string, Entry> database_;
    mutable std::mutex mutex_;
};