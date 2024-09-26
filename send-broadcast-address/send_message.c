#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BROADCAST_PORT 49152
#define BROADCAST_IP 
#define MESSAGE "Hello, good afternoon"

int main(int argc, char *argv[]){
    int sockfd;
    struct sockaddr_in broadcast_addr;
    int broadcast_permission = 1;
    int send_result;

    // Create a UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0){
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    if(setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast_permission, sizeof(broadcast_permission)) < 0){
        perror("Error in setting broadcast option");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    memset(&broadcast_addr, 0, sizeof(broadcast_addr));
    broadcast_addr.sin_family = AF_INET;
    broadcast_addr.sin_port = htons(BROADCAST_PORT);
    broadcast_addr.sin_addr.s_addr = inet_addr(BROADCAST_IP);
    
    send_result = sendto(sockfd, MESSAGE, sizeof(MESSAGE), 0, (struct sockaddr*)&broadcast_addr, len(broadcast_addr));
    if(send_result < 0) {
        perror("Send message failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Broadcast message sent to %s\n", BROADCAST_IP);
    close(sockfd);
    return 0;
}