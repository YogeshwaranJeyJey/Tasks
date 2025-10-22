#include <stdio.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define BUFFERSIZE  1024
#define PORT    8080
#define BACKLOG 5

int main(){
    int server_fd, new_socket, opt = 1, message_count = 0;
    struct sockaddr_in address;
    socklen_t addressLength = sizeof(address);
    char buffer[BUFFERSIZE];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd < 0){
        perror("Server Socket Creation Failed!");
        exit(EXIT_FAILURE);
    }

    if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))){
        perror("Setsockopt Failed!");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if(bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0){
        perror("Bind Failed!");
        exit(EXIT_FAILURE);
    }

    if(listen(server_fd, BACKLOG) < 0){
        perror("Listen Failed!");
        exit(EXIT_FAILURE);
    }

    printf("Server Listening on port : %d...\n", PORT);

    new_socket = accept(server_fd, (struct sockaddr*)&address, &addressLength);
    if (new_socket < 0) {
        perror("Accept Failed!");
        exit(EXIT_FAILURE);
    }

    printf("Client Connected!\n");

    while(1){
        memset(buffer, 0, sizeof(buffer));
        int valRead  = read(new_socket, buffer, sizeof(buffer));
        if(valRead <= 0){
            printf("Client Disconnected!\n");
            break;
        }

        message_count++;
        printf("Server Received Message %d: %s\n", message_count, buffer);

        char ack[50];
        sprintf(ack, "Server Acknowledged message %d!", message_count);
        send(new_socket, ack, strlen(ack), 0);
    }
    close(new_socket);
    close(server_fd);
    return 0;
}