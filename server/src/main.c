#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsa_data;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server, client_addr;
    int addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
        printf("Failed to initialize Winsock. Error Code: %d\n", WSAGetLastError());
        return EXIT_FAILURE;
    }

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET) {
        printf("Failed to create socket. Error Code: %d\n", WSAGetLastError());
        WSACleanup();
        return EXIT_FAILURE;
    }

    // Setup server address struct
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1"); // localhost
    server.sin_port = htons(PORT);

    // Bind
    if (bind(server_socket, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Bind failed. Error Code: %d\n", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        return EXIT_FAILURE;
    }

    // Listen
    if (listen(server_socket, 3) == SOCKET_ERROR) {
        printf("Listen failed. Error Code: %d\n", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        return EXIT_FAILURE;
    }

    printf("Waiting for connections...\n");

    // Accept a client socket
    client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &addr_len);
    if (client_socket == INVALID_SOCKET) {
        printf("Accept failed. Error Code: %d\n", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        return EXIT_FAILURE;
    }

    // Receive data from client
    int bytesReceived = recv(client_socket, buffer, BUFFER_SIZE, 0);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0'; // Null-terminate the received data
        printf("Received from client: %s\n", buffer);
    }

    // Send a response to client
    const char *response = "Hello from server";
    send(client_socket, response, strlen(response), 0);

    // Cleanup
    closesocket(client_socket);
    closesocket(server_socket);
    WSACleanup();
    return EXIT_SUCCESS;
}