#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define connections 5

int main(int argc, char *argv[]){
    int sockfd, newsockfd, portno, cli_len, n;
    struct sockaddr_in serv_addr, cli_addr;
    char buffer[256];

    if(argc < 2){
        fprintf(stderr, "Error, no port provided,\n");
        exit(EXIT_FAILURE);
    }

    // Create TCP socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset((char *)&serv_addr, 0, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0){
        perror("Error on binding");
        exit(EXIT_FAILURE);
    }
    listen(sockfd, connections);
    cli_len = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &cli_addr);

    if (newsockfd < 0){
        perror("Error on accept");
        exit(EXIT_FAILURE);
    }

    while(1){
        memset(buffer, 0, sizeof(buffer));
        n = read(newsockfd, buffer, sizeof(buffer) - 1);
        if(n < 0){
            perror("Error on reading");
            break;
        }else if(n == 0){
            printf("Client closed the connection\n");
            break;
        }
        printf("incoming message from client: %s\n", buffer);

        memset(buffer, 0, sizeof(buffer));
        printf("Enter response: ");
        if(fgets(buffer, sizeof(buffer), stdin) == NULL){
            perror("Error on writing");
            close(newsockfd);
            exit(EXIT_FAILURE);
        }
        n = write(newsockfd, buffer, strlen(buffer));
        if(n < 0){
            perror("Error on writing");
            break;
        }
    }
    
    close(newsockfd);
    return 0;
}