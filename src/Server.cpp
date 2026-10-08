#include "Server.h"
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "CommandParser.h"
#include "CommandExecutor.h"
#include "Persistence.h"
#include <thread>
#include <chrono>
#include "RESPParser.h"
#include "RESPEncoder.h"
#include "ClientSession.h"
#include <utility>

Server::Server(int port, std::string password)
    : port_(port),
      password_(std::move(password))
{
}
bool Server::initializeWinsock()
{
    WSADATA wsaData;

    int result =
        WSAStartup(MAKEWORD(2,2), &wsaData);

    if(result != 0)
    {
        std::cout << "WSAStartup failed.\n";
        return false;
    }

    std::cout << "Winsock initialized successfully.\n";

    return true;
}
bool Server::createServerSocket()
{
    serverSocket_ = socket(
        AF_INET,
        SOCK_STREAM,
        IPPROTO_TCP);

    if (serverSocket_ == INVALID_SOCKET)
    {
        std::cout << "Socket creation failed.\n";

        WSACleanup();

        return false;
    }

    return true;
}
bool Server::bindSocket()
{
    sockaddr_in serverAddress{};

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port_);

    int bindResult = bind(
        serverSocket_,
        reinterpret_cast<sockaddr*>(&serverAddress),
        sizeof(serverAddress));


    if (bindResult == SOCKET_ERROR)
    {
        std::cout << "Bind failed: "
                  << WSAGetLastError()
                  << std::endl;

        closesocket(serverSocket_);
        WSACleanup();

        return false;
    }

    return true;
}
bool Server::startListening()
{
    int listenResult = listen(
        serverSocket_,
        SOMAXCONN);


    if (listenResult == SOCKET_ERROR)
    {
        std::cout << "Listen failed: "
                  << WSAGetLastError()
                  << std::endl;

        closesocket(serverSocket_);
        WSACleanup();

        return false;
    }

    std::cout << "Listening on port "
              << port_
              << "...\n";

    std::cout << "Waiting for clients...\n";

    return true;
}
bool Server::start()
{
    std::cout << "====================================\n";
    std::cout << "         MiniRedis Server\n";
    std::cout << "====================================\n";

    if (!initializeWinsock())
        return false;

    if (!createServerSocket())
        return false;

    if (!bindSocket())
        return false;

    if (!startListening())
        return false;

        Persistence::load(store_, "database.txt");

        std::thread cleanupThread([this]()
        {
            while (true)
            {
                store_.removeExpiredKeys();
        
                std::this_thread::sleep_for(
                    std::chrono::seconds(1));
            }
        });
        
        cleanupThread.detach();
        acceptClients();

    closesocket(serverSocket_);
    WSACleanup();

    return true;
}
void Server::acceptClients()
{
  

    while (true)
    {
        std::cout << "Waiting for clients...\n";

        SOCKET clientSocket = accept(
            serverSocket_,
            nullptr,
            nullptr);

        if (clientSocket == INVALID_SOCKET)
        {
            std::cout << "Accept failed: "
                      << WSAGetLastError()
                      << std::endl;

            continue;
        }

        std::thread clientThread(
            &Server::handleClient,
            this,
            clientSocket);
        
        clientThread.detach();
    }
}
void Server::handleClient(SOCKET clientSocket)
{
    std::cout << "Client connected!\n";
    ClientSession session;

    stats_.clientConnected();
    

    CommandExecutor executor(
        store_,
        stats_,
        &session,
        password_);

    constexpr int BUFFER_SIZE = 1024;
    while (true)
    {
       

char buffer[BUFFER_SIZE]{};

        int bytesReceived = recv(
            clientSocket,
            buffer,
            sizeof(buffer) - 1,
            0);

        if (bytesReceived <= 0)
        {
            break;
        }

        buffer[bytesReceived] = '\0';

                  std::vector<std::string> tokens;

                  const bool isRESP = (buffer[0] == '*');

                  if (isRESP)
                  {
                      RESPParser parser;
                      tokens = parser.parse(buffer);
                  }
                  else
                  {
                      CommandParser parser;
                      tokens = parser.parse(buffer);
                  }
                 

                  std::string response =
                      executor.execute(tokens);
                  
                  if (isRESP)
                  {
                      if (response == "OK")
                      {
                          response =
                              RESPEncoder::simpleString("OK");
                      }
                      else if (response == "PONG")
                      {
                          response =
                              RESPEncoder::simpleString("PONG");
                      }
                      else if (response == "(nil)")
                      {
                          response =
                              RESPEncoder::nullBulkString();
                      }
                      else if (response.rfind("ERR", 0) == 0 ||
                               response.rfind("NOAUTH", 0) == 0)
                      {
                          // Errors use the RESP error type ("-...")
                          response =
                              RESPEncoder::error(response);
                      }
                      else
                      {
                          response =
                              RESPEncoder::bulkString(response);
                      }
                  }

                  int bytesSent =
                  send(
                      clientSocket,
                      response.c_str(),
                      static_cast<int>(response.size()),
                      0);
              
              if (bytesSent == SOCKET_ERROR)
              {
                  std::cerr
                      << "Send failed: "
                      << WSAGetLastError()
                      << '\n';
              
                  break;
              }
    }
    stats_.clientDisconnected();
    closesocket(clientSocket);

    std::cout << "Client disconnected.\n";
}