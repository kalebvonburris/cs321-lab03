// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define PORT 8080
   
int main(int argc, char const *argv[])
{
    int server_socket= 0, valread;
    struct sockaddr_in serv_addr;
    pid_t pid = getpid();

    char *pid_buffer = malloc(4);

    pid_buffer[0] = pid >> 24;
    pid_buffer[1] = pid >> 16;
    pid_buffer[2] = pid >> 8;
    pid_buffer[3] = pid;

    if ((server_socket= socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
   
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
       
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
   
    if (connect(server_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    // Connection send
    char buffer[1024] = {0};

    read(server_socket, buffer, sizeof(buffer));

    printf("Server: %s\n", buffer);

    while(buffer[0] != 1) {
        memset(buffer, 0, 1);
        read(server_socket, buffer, sizeof(buffer));
    }

    
    
    // Loop user input
    while(1) {
        memset(buffer, 0, sizeof(buffer));

        printf("Enter a string: ");
        fgets(buffer, 1024, stdin);

        send(server_socket, buffer, sizeof(buffer), 0);

        int8_t response_buffer[1024] = {0};

        valread = read(server_socket, response_buffer, sizeof(response_buffer));

        printf("Server: %s\n", response_buffer);
    }

    return 0;
}
