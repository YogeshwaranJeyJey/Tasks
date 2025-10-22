#include <stdio.h>
#include <errno.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFERSIZE  1024
#define SERVERIP    "127.0.0.1"
#define PORT    8080

int main(){
    int client_fd;
    struct sockaddr_in serverAddress;
    char buffer[BUFFERSIZE] = {0};

    client_fd = socket(AF_INET, SOCK_STREAM, 0);

    if(client_fd < 0){
        perror("Client Socket creation Failed!\n");
        exit(EXIT_FAILURE);
    }

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);

    if(inet_pton(AF_INET, SERVERIP, &serverAddress.sin_addr) <= 0){
        perror("Invalid address / Address not supported");
        exit(EXIT_FAILURE);
    }

    if(connect(client_fd, (struct sockaddr* )&serverAddress, sizeof(serverAddress)) < 0){
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

     printf("Connected to server at %s:%d\n", SERVERIP, PORT);

     for(int i = 1; i <= 5; i++){
        char msg[BUFFERSIZE] = {0};
        sprintf(msg, "This is message %d", i);
        send(client_fd, msg, strlen(msg), 0);
        printf("Sent: %s\n", msg);

        memset(buffer, 0, sizeof(buffer));
        read(client_fd, buffer, sizeof(buffer));
        printf("Server: %s\n", buffer);

        sleep(2);
     }
     printf("All messages sent!\n");
     close(client_fd);
     return 0;
}