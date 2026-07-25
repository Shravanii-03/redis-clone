#include "ClientSession.h"

ClientSession::ClientSession()
{
    authenticated_ = false;
}

bool ClientSession::isAuthenticated() const
{
    return authenticated_;
}

void ClientSession::authenticate()
{
    authenticated_ = true;
}