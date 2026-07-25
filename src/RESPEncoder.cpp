#include "RESPEncoder.h"

std::string RESPEncoder::simpleString(
    const std::string& value)
{
    return "+" + value + "\r\n";
}

std::string RESPEncoder::error(
    const std::string& value)
{
    return "-" + value + "\r\n";
}

std::string RESPEncoder::bulkString(
    const std::string& value)
{
    return "$" +
           std::to_string(value.size()) +
           "\r\n" +
           value +
           "\r\n";
}

std::string RESPEncoder::nullBulkString()
{
    return "$-1\r\n";
}

std::string RESPEncoder::integer(int value)
{
    return ":" +
           std::to_string(value) +
           "\r\n";
}