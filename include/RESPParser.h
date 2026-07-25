#pragma once

#include <string>
#include <vector>

class RESPParser
{
public:
    std::vector<std::string> parse(const std::string& request);
};