#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 5000

int main() {
    int sock = 0;
    struct sockaddr_in server_addr;
    char buffer[1024] = {0};

    char *message = "Hello, everyone!";

    // Step 1: Create a socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket creation error");
        return -1;
    }

    // Step 2: Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    // Convert IP address from text to binary
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        perror("Invalid address or address not supported");
        return -1;
    }

    // Step 3: Connect to the server
    if (connect(sock, (struct sockaddr *)&server_addr,
        sizeof(server_addr)) < 0) {
        perror("Connection failed");
        return -1;
    }
    printf("Connected to chat server.\n");

    // Step 4: Send data to the server
    send(sock, message, strlen(message), 0);
    printf("%s\n", message);

    // Step 5: Close the socket
    close(sock);
    
    return 0;
}
