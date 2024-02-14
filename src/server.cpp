#include <iostream>
#include <winsock2.h>

#include "constants.hpp"

void handle_client(SOCKET& client_socket);

int main() {
    WSADATA wsadata;
    if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0) {
        std::cerr << "failed to initialize winsock2\n";
        return 1;
    }

    SOCKET server_socket;
    server_socket = socket(AF_INET, SOCK_STREAM, 0); // use IPv4 and TCP
    if (server_socket == INVALID_SOCKET) {
        std::cerr << "failed to create socket: " << WSAGetLastError() << '\n';
        WSACleanup();
        return 1;
    }

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ADDRESS);
    server_addr.sin_port = htons(PORT);

    int resp = bind(server_socket, (SOCKADDR*)&server_addr, sizeof(server_addr));
    if (resp == SOCKET_ERROR) {
        std::cerr << "failed to bind to socket: " << WSAGetLastError() << '\n';
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    std::cout << "bound to address: " << inet_ntoa(server_addr.sin_addr) << '\n';

    resp = listen(server_socket, SOMAXCONN);
    if (resp == SOCKET_ERROR) {
        std::cerr << "failed to listen on port: " << PORT << '\n' << WSAGetLastError() << '\n';
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    std::cout << "listening on port: " << PORT << '\n';

    // accept incoming connection requests
    // halts program execution until a client connects
    SOCKET client_socket = accept(server_socket, nullptr, nullptr);
    if (client_socket == INVALID_SOCKET) {
        std::cerr << "could not accept connection request: " << WSAGetLastError() << '\n';
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    std::cout << "client connected\n";
    handle_client(client_socket);

    std::cout << "closing server";
    closesocket(server_socket);
    closesocket(client_socket);
    WSACleanup();
    return 0;
}

void handle_client(SOCKET& client_socket) {
    bool running = true;
    do {
        char buffer[1024]{}; 
        int resp = recv(client_socket, buffer, sizeof(buffer), 0);
        if (resp > 0) {
            std::cout << "message from client: " << buffer << '\n';
        } else if (resp == 0) {
            // client disconnected
            running = false;
        } else {
            std::cerr << "error receiving data: " << WSAGetLastError() << '\n';
            running = false;
        }
    } while (running);
}