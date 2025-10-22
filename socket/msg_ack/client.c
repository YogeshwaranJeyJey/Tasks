#include <stdio.h>
#include <errno.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFERSIZE  1024
#define SERVERIP    "127.0.0.1"
#define PORT    8080
#define MSGCOUNT    10

int main(){
    int client_fd, ack_count = 0;
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

     for(int i = 1; i <= MSGCOUNT; i++){
        char msg[BUFFERSIZE] = {0};
        sprintf(msg, "This is message %d", i);
        send(client_fd, msg, strlen(msg), 0);
        printf("Sent: %s\n", msg);

        memset(buffer, 0, sizeof(buffer));
        int valRead = read(client_fd, buffer, sizeof(buffer));
        if(valRead > 0){
            printf("Server: %s\n", buffer);
            ack_count++;
        }
        else{
            printf("Failed to receive acknowledgement for message : %s\n", buffer);
            exit(EXIT_FAILURE);
        }
        sleep(2);
     }
     printf("All messages sent!\n");
     printf("Total acknowledgement count: %d\n", ack_count);
     if(MSGCOUNT == ack_count)
        printf("All the %d Messages were acnowledged by the server!\n", ack_count);
    else
        printf("Only %d Messages were acnowledged by the server!\n", ack_count);

     close(client_fd);
     return 0;
}