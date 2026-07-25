#pragma once

#include <string>

class RESPEncoder
{
public:
    static std::string simpleString(const std::string& value);
    static std::string error(const std::string& value);
    static std::string bulkString(const std::string& value);
    static std::string nullBulkString();
    static std::string integer(int value);
};