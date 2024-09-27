#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

void error(char *msg){
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]){
    int sockfd;
    struct sockaddr_in serv_addr;
    char buffer[INET_ADDRSTRLEN]; // To store the IP address in human-readable format

    // Create a UDP socket (SOCK_DGRAM)
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0)
        error("Socket creation failed");
    
    //Setting up the server address to Google's public DNS server (8.8.8.8) at port 53 (DNS port)
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(53); // DNS port
    inet_pton(AF_INET, "8.8.8.8", &serv_addr.sin_addr);

    // Connect the socket to the server
    // No actual data will be sent, this is just used to get the local IP address
    if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("Connection failed");
    
    // Get the local address (the one used to connect to 8.8.8.8)
    struct sockaddr_in local_addr;
    socklen_t addr_len = sizeof(local_addr);
    if(getsockname(sockfd, (struct sockaddr *)&local_addr, &addr_len) < 0)
        error("getsockname failed");

    // Convert the IP address from binary to text form
    inet_ntop(AF_INET, &local_addr.sin_addr, buffer, sizeof(buffer));

    // Print the local IP address
    printf("Local IP address: %s\n", buffer);

    // Close the socket
    close(sockfd);

    return 0;
}