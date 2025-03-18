// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#define PORT 8080
int main(int argc, char const *argv[])
{
    int server_fd, server_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    // pid_t = 32bits, char = 8bits
    pid_t users[2];
       
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
       
    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
       
    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, 
                                 sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((server_socket = accept(server_fd, (struct sockaddr *)&address, 
                       (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    // Client (data) -> Server -> read() -> data = [pid, [chars]]

    while(1) {
        valread = read(server_socket , buffer, sizeof(buffer));

        pid_t read_pid = buffer[0] << 24 | buffer[1] << 16 | buffer[2] << 8 | buffer[3];

        if (read_pid == 0) {
            printf("No pid\n");
            continue;
        }

        // Handle a PID not seen before
        if (read_pid != users[0] || read_pid != users[1]) {
            if (users[0] == 0) {
                users[0] = read_pid;
            } else if (users[1] == 0) {
                users[1] = read_pid;
            } else {
                printf("Too many users\n");
                char* message = "Only one client is up.";
                send(server_socket, message , strlen(message) , 0 );
                continue;
            }
        }

        if (users[0] == 0 || users[1] == 0) {
            printf("Only one user connected.\n");
            send(server_socket , buffer , strlen(buffer) , 0 );
            continue;
        }

        printf("pid: %x\n", read_pid);
        printf("buffer data: %x %x %x %x\n", buffer[0], buffer[1], buffer[2], buffer[3]);


        
        memset(buffer, 0, sizeof(buffer));
    }

    return 0;
}

