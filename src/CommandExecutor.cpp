#include "CommandExecutor.h"
#include "Persistence.h"

#include <cctype>
#include <utility>

CommandExecutor::CommandExecutor(
    DataStore& store,
    ServerStats& stats,
    ClientSession* session,
    std::string password)
    : store_(store),
      stats_(stats),
      session_(session),
      password_(std::move(password))
{
}

std::string CommandExecutor::execute(
    const std::vector<std::string>& rawTokens)
{
    if(rawTokens.empty())
        return "ERR Empty command";

    // Command names are case-insensitive (like Redis); arguments are not.
    std::vector<std::string> tokens = rawTokens;
    for (char& c : tokens[0])
        c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));

    // ---------------- Authentication ----------------
    const bool authRequired = !password_.empty() && session_ != nullptr;

    if (tokens[0] == "AUTH")
    {
        if (!authRequired)
            return "ERR Client sent AUTH, but no password is set";

        if (tokens.size() != 2)
            return "ERR Usage: AUTH password";

        if (tokens[1] == password_)
        {
            session_->authenticate();
            return "OK";
        }

        return "ERR invalid password";
    }

    if (authRequired && !session_->isAuthenticated())
        return "NOAUTH Authentication required";

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