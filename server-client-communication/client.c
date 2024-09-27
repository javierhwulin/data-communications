#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

int main(int argc, char *argv[]){
    int sockfd, portno, n;
    char buffer[256];
    struct sockaddr_in serv_addr;
    struct hostent *server;
    int rcvbuf_size = 1024 * 1024; // 1 MB buffer size

    if(argc < 3){
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        perror("Error on creating socket");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &rcvbuf_size, sizeof(rcvbuf_size)) < 0) {
        perror("Error setting SO_RCVBUF");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    server = gethostbyname(argv[1]);
    if(server == NULL){
        perror("Error, no such host");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    portno = atoi(argv[2]);
    
    memset((char *)&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    memcpy(&serv_addr.sin_addr.s_addr, server->h_addr_list[0], server->h_length);
  
    if(connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0){
        perror("Error on connect");
        exit(EXIT_FAILURE);
    }

    while(1){
        memset(buffer, 0, sizeof(buffer));
        printf("Enter response: ");
        if(fgets(buffer, sizeof(buffer), stdin) == NULL){
            perror("Error on writing");
            exit(EXIT_FAILURE);
        }
        n = write(sockfd, buffer, strlen(buffer));
        if(n < 0){
            perror("Error on writing");
            exit(EXIT_FAILURE);
        }else if(n == 0){
            perror("Closing connection");
            break;
        }

        memset(buffer, 0, sizeof(buffer));
        n = read(sockfd, buffer, sizeof(buffer) - 1);
        if(n < 0){
            perror("Error on reading");
            exit(EXIT_FAILURE);
        }else if(n == 0){
            perror("Server closed connection");
            break;
        }
        printf("incoming message from server: %s\n", buffer);
    }   

    return 0;
}