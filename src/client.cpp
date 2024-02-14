#include <iostream>
#include <winsock2.h>
#include <string>

#include "constants.hpp"

void handle_server(SOCKET& client_socket);

int main() {
    WSADATA wsadata;
    if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0) {
        std::cerr << "failed to initialize winsock2\n";
        return 1;
    }

    SOCKET client_socket;
    client_socket = socket(AF_INET, SOCK_STREAM, 0); // use IPv4 and TCP
    if (client_socket == INVALID_SOCKET) {
        std::cerr << "failed to create socket: " << WSAGetLastError() << '\n';
        WSACleanup();
        return 1;
    }

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ADDRESS);
    server_addr.sin_port = htons(PORT);

    int resp = connect(client_socket, (SOCKADDR*)&server_addr, sizeof(server_addr));
    if (resp == SOCKET_ERROR) {
        std::cerr << "failed to connect to server: " << WSAGetLastError() << '\n';
        closesocket(client_socket);
        WSACleanup();
        return 1;
    }

    std::cout << "connected to server: " << ADDRESS << '\n';

    handle_server(client_socket);

    std::cout << "closing client";
    closesocket(client_socket);
    WSACleanup();
    return 0;
}

void handle_server(SOCKET& client_socket) {
    bool running = true;
    while (running) {
        std::cout << ">>";
        std::string u_in;
        std::getline(std::cin, u_in);
        if (u_in == "exit") {
            running = false;
        }
        else {
            const char* msg = u_in.c_str();
            size_t msg_len = u_in.length();
            if (send(client_socket, msg, strlen(msg), 0) == SOCKET_ERROR) {
                std::cerr << "failed to send data to server: " << WSAGetLastError() << '\n';
            }
        }
    }
}