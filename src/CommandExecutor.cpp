#include "CommandExecutor.h"
#include "Persistence.h"

CommandExecutor::CommandExecutor(
    DataStore& store,
    ServerStats& stats)
    : store_(store),
      stats_(stats)
{
}

std::string CommandExecutor::execute(
    const std::vector<std::string>& tokens)
{
    if(tokens.empty())
        return "ERR Empty command";

    if(tokens[0] == "PING")
        return "PONG";

        if (tokens[0] == "SET")
        {
            // Normal SET
            if (tokens.size() == 3)
            {
                store_.set(tokens[1], tokens[2]);
                return "OK";
            }
        
            // SET key value EX seconds
            if (tokens.size() == 5 &&
                tokens[3] == "EX")
            {
                int ttl = std::stoi(tokens[4]);
        
                store_.set(
                    tokens[1], tokens[2], ttl);
        
                return "OK";
            }
        
            return "ERR Usage: SET key value [EX seconds]";
        }

    if(tokens[0] == "GET")
    {
        if(tokens.size()!=2)
            return "ERR Usage: GET key";

        return store_.get(tokens[1]);
    }


    if (tokens[0] == "DEL")
{
    if (tokens.size() != 2)
    {
        return "ERR Usage: DEL key";
    }

    bool deleted = store_.del(tokens[1]);

    if (deleted)
        return "1";

    return "0";
}
if (tokens[0] == "KEYS")
{
    auto allKeys = store_.keys();

    if (allKeys.empty())
    {
        return "(empty)";
    }

    std::string response;

    for (const auto& key : allKeys)
    {
        response += key + "\n";
    }

    return response;
}
if (tokens[0] == "SAVE")
{
    if (Persistence::save(store_, "database.txt"))
    {
        return "OK";
    }

    return "ERR Save failed";
}
if (tokens[0] == "INFO")
{
    std::string info;

    info += "MiniRedis Server\n";
    info += "----------------\n";

    info += "Keys: ";
    info += std::to_string(store_.size());
    info += "\n";

    info += "Connected Clients: ";
    info += std::to_string(stats_.clientCount());
    info += "\n";

    info += "Uptime: ";
    info += std::to_string(stats_.uptimeSeconds());
    info += " seconds\n";

    return info;
}
    return "ERR Unknown command";
}