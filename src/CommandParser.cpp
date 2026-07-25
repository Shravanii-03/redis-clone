#include "CommandParser.h"

#include <sstream>

std::vector<std::string> CommandParser::parse(const std::string& input)
{
    std::stringstream ss(input);
    std::vector<std::string> tokens;
    std::string word;
    while (ss >> word)
    {
        tokens.push_back(word);
    }

    return tokens;
}