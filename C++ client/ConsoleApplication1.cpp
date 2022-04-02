#include <iostream>
#include <WinSock2.h>
#include <thread>
#include <list>
#include <vector>
#include <WS2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

std::list<SOCKET> ClientList;

void Run(SOCKET);

int main()
{
    WSADATA data;
    int e = WSAStartup(MAKEWORD(2, 2), &data);
    if (e != 0) {
        std::cout << "";
    }

    SOCKET ServerSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ServerSocket < 0) {

        std::cout << "Socket error1" << std::endl;
        return 1;
    }

    struct sockaddr_in serverAdress;
    memset(&serverAdress, 0x0, sizeof(serverAdress));
    serverAdress.sin_family = AF_INET;
    serverAdress.sin_port = htons(8080);
    serverAdress.sin_addr.S_un.S_un_b.s_b1 = 127;
    serverAdress.sin_addr.S_un.S_un_b.s_b2 = 0;
    serverAdress.sin_addr.S_un.S_un_b.s_b3 = 0;
    serverAdress.sin_addr.S_un.S_un_b.s_b4 = 1;
    int result = bind(
        ServerSocket,
        (const sockaddr*)(&serverAdress),
        sizeof(serverAdress)
    );
    if (result == SOCKET_ERROR) {

        std::cout << "Socket error2" << std::endl;
        return 1;
    }
    result = listen(ServerSocket, 100);
    if (result < 0) {
        std::cout << "Socket error3" << std::endl;
        return 1;
    }
    while (true) {
        std::cout << "wait client connection\n";
        SOCKET clientSocket = accept(ServerSocket, nullptr, nullptr);
        std::cout << "a new client connected\n";
        new std::thread(Run, clientSocket);
        ClientList.push_back(clientSocket);
    }
    return 0;
}
void Run(SOCKET s) {
    char message[512];
    int n = recv(s, message, 512, 0);
    if (n < 0) {
        return;
    }
    std::cout << message << std::endl;
    send(s, "pnog", 5, 0);
    if (n < 0) {
        return;
    }
    while (true) {
        int n = recv(s, message, 512, 0);
        if (n < 0) {
            return;
        }
    }
   // ClientList.remove(s);
}