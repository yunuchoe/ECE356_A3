    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <unistd.h>
    #include <pthread.h>
    #include <arpa/inet.h>

    #define PORT 5000
    #define BUFFER_SIZE 1024
    #define MAX_CLIENTS 2

    int client_count = 0; // not as useful in my version

    void *handle_client(void *client_socket);
    
    int main() {
        int server_fd;
        int new_socket[MAX_CLIENTS]; // make this an array so it can be stored and tied to a client
        struct sockaddr_in server_addr, client_addr;

        socklen_t addr_len = sizeof(client_addr);
        pthread_t client_threads[MAX_CLIENTS];

        // Step 1: Create a socket
        server_fd = socket(AF_INET, SOCK_STREAM, 0);

        // Step 2: Configure server address
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = INADDR_ANY;
        server_addr.sin_port = htons(PORT);

        // Step 3: Bind the socket to the specified IP and port
        bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));

        // Step 4: Set the socket to listen for incoming connections
        listen(server_fd, MAX_CLIENTS);
        printf("Chat sever started. Waiting for clients...\n");
        for(int i = 0; i < MAX_CLIENTS; i++){ // used i instead of client count as I changed its purpose
            new_socket[i] = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);
            // printf the client connected but dont do anyting else
            // wait for all clients to join
            printf("New client conneceted: Client %d\n", i + 1);
        }

        // At this point, we know each client has connected
        // thus they can at the same time do their job (send message)
        // Step 5: Handle the client in a new thread
        for(int i = 0; i < MAX_CLIENTS; i++){ // loop through and run each thread
            // handle errors if needed
            if(pthread_create(&client_threads[i], NULL, handle_client, (void *)&new_socket[i])!= 0) {
                perror("Thread creation failed");
                return 1;
            }
            client_count++; // client count is used here to trasnmit to the function which client is speaking
        }
        
        // Wait for all threads to finish
        for (int i = 0; i < MAX_CLIENTS; i++) {
            pthread_join(client_threads[i], NULL);
        }
        
        // Step 6: Close the server socket
        close(server_fd);

        return 0;
    }
        
    // Thread function to handle clientcommunication
    void *handle_client(void *client_socket) {
        int socket = *(int *)client_socket;
        char buffer[BUFFER_SIZE] = {0};
        char *response = "Hello from Server";
        int valread;

        // Step 7: Receive data from the client
        valread = read(socket, buffer, BUFFER_SIZE);
        if (valread > 0) {

            printf("Client %d: %s\n", client_count, buffer);

            // Step 8: Send response to the client
            send(socket, response, strlen(response), 0);
        }

        // Step 9: Close the client socket
        close(socket);
        printf("Client %d disconnected.\n", client_count);
        
        pthread_exit(NULL);
    }