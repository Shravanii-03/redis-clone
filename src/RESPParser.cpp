#include "RESPParser.h"

#include <sstream>

std::vector<std::string> RESPParser::parse(
    const std::string& request)
{
    std::vector<std::string> tokens;

    std::stringstream ss(request);

    std::string line;

    // Read and ignore the first line (*3)
    std::getline(ss, line);

    while (std::getline(ss, line))
    {
        // Skip empty lines
        if (line.empty())
            continue;

        // Remove '\r' if present
        if (!line.empty() && line.back() == '\r')
        {
            line.pop_back();
        }

        // Skip lines like $3, $4, $8
        if (!line.empty() && line[0] == '$')
            continue;

        // Add actual command/arguments
        tokens.push_back(line);
    }

    return tokens;
}