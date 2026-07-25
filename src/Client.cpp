#include <iostream>
#include <winsock2.h>
#include <string>

int main()
{
    WSADATA wsaData;

    WSAStartup(MAKEWORD(2,2), &wsaData);

    SOCKET client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    sockaddr_in server{};

    server.sin_family = AF_INET;
    server.sin_port = htons(6379);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    if(connect(client,
               (sockaddr*)&server,
               sizeof(server)) == SOCKET_ERROR)
    {
        std::cout << "Connection failed\n";
        return 1;
    }

    std::cout << "Connected!\n";

    while (true)
    {
        std::string input;
    
        std::cout << "MiniRedis> ";
    
        std::getline(std::cin, input);
    
        if (input == "exit")
            break;
    
        send(
            client,
            input.c_str(),
            input.length(),
            0);
    
        char buffer[1024] = {0};
    
        int bytesReceived = recv(
            client,
            buffer,
            sizeof(buffer) - 1,
            0);
    
        if (bytesReceived <= 0)
        {
            std::cout << "Server disconnected.\n";
            break;
        }
    
        buffer[bytesReceived] = '\0';
    
        std::cout << "Server replied: "
                  << buffer
                  << std::endl;
    }
    
    closesocket(client);
    WSACleanup();
}