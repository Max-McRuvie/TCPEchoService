#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main(){
    WSADATA wsa_data;
    SOCKET client_socket;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
        printf("Failed to initialize Winsock. Error Code: %d\n", WSAGetLastError());
        return EXIT_FAILURE;
    }

    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == INVALID_SOCKET) {
        printf("Failed to create socket. Error Code: %d\n", WSAGetLastError());
        WSACleanup();
        return EXIT_FAILURE;
    }

    // Setup server address struct
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Change this if the server is on a different machine

    // Connect to server
    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("Connection failed. Error Code: %d\n", WSAGetLastError());
        closesocket(client_socket);
        WSACleanup();
        return EXIT_FAILURE;
    }

    // Send data to server
    const char *message = "Hello from client";
    send(client_socket, message, strlen(message), 0);

    // Receive response from server
    int bytesReceived = recv(client_socket, buffer, BUFFER_SIZE, 0);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0'; // Null-terminate the received data
        printf("Received from server: %s\n", buffer);
    }

    // Cleanup
    closesocket(client_socket);
    WSACleanup();
    return EXIT_SUCCESS;
}