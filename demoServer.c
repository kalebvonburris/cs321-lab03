// Kaleb Burris & Kylie Lambries
// CS 321
// 03/17/2025
// Server side C program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <signal.h>

#define PORT 8080
int main(int argc, char const *argv[])
{
    signal(SIGINT, SIG_IGN);

    int server_fd, server_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    // pid_t = 32bits, char = 8bits
    pid_t users[2] = {0, 0};
       
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
       
    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
    
     // Forcefully attaching socket to the port 8080
     if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 2) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // Connect client 1
    int client_1 = accept(server_fd, (struct sockaddr *)&address,
        (socklen_t*)&addrlen); 
    if (client_1 < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    // Send required message to the first client
    char* message = "Only one client is up.";
    send(client_1, message, strlen(message), 0);

    // Connect client 2
    int client_2 = accept(server_fd, (struct sockaddr *)&address, 
        (socklen_t*)&addrlen); 
    if (client_2 < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    message =  "Both clients connected!";
    send(client_2, message, strlen(message), 0);

    // Send connection accepted signal to both clients
    int connection[1] = {1};
    send(client_1, connection, sizeof(connection), 0);
    send(client_2, connection, sizeof(connection), 0);

    while (1) {
        char buffer_1[1024] = {0};
        read(client_1, buffer_1, sizeof(buffer_1));

        printf("Client 1: %s\n", buffer_1);

        if (buffer_1[0] != 0) {
            if (strcmp(buffer_1, "BYE\n") == 0) {
                exit(0);
            }

            send(client_2, buffer_1, strlen(buffer_1), 0);
        }

        memset(buffer_1, 0, sizeof(buffer_1));

        printf("Client 2: %s\n", buffer_1);

        char buffer_2[1024] = {0};
        read(client_2, buffer_2, sizeof(buffer_1));
        if (buffer_2[0] != 0) {
            if (strcmp(buffer_2, "BYE\n") == 0) {
                exit(0);
            }
            send(client_1, buffer_2, strlen(buffer_2), 0);
        }

        memset(buffer_2, 0, sizeof(buffer_1));
    }

    return 0;
}

