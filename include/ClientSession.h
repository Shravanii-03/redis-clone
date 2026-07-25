#pragma once

class ClientSession
{
public:
    ClientSession();

    bool isAuthenticated() const;

    void authenticate();

private:
    bool authenticated_;
};