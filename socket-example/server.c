#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

// Function to handle errors by printing the error message and exiting the program
void error(char *msg){
    perror(msg);  // Print the error message
    exit(1);      // Exit the program with a non-zero status
}

int main(int argc, char *argv[]){
    int sockfd, newsockfd, portno, clilen;  // Socket descriptors and port number
    char buffer[256];                       // Buffer to hold the data received from the client
    struct sockaddr_in serv_addr, cli_addr; // Structures for server and client address
    int n;                                  // Variable to store the number of bytes read/written

    // Check if the port number is provided as an argument
    if (argc < 2){
        fprintf(stderr, "Error, no port provided,\n");
        exit(1);
    }

    // Create a socket:
    // AF_INET -> IPv4, SOCK_STREAM -> TCP, 0 -> default protocol (TCP)
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)        // If socket creation failed
        error("Error opening socket");

    // Initialize server address structure
    bzero((char *) &serv_addr, sizeof(serv_addr)); // Zero out the server address structure
    portno = atoi(argv[1]);  // Convert the provided port number from string to integer
    serv_addr.sin_family = AF_INET; // Set the address family to IPv4
    serv_addr.sin_port = htons(portno); // Convert port number to network byte order
    serv_addr.sin_addr.s_addr = INADDR_ANY; // Allow connections from any IP address

    // Bind the socket to the specified port and address
    if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0 )
        error("Error on binding");

    // Start listening for incoming connections:
    // 5 -> backlog size (maximum number of pending connections)
    listen(sockfd, 5);

    // Accept a connection from a client
    clilen = sizeof(cli_addr);  // Set the size of the client address structure
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen); // Accept a connection
    if(newsockfd < 0)           // If the accept call fails
        error("Error on accept");

    // Clear the buffer for receiving the message
    bzero(buffer, 256);

    // Read the message sent by the client into the buffer
    n = read(newsockfd, buffer, 255); // Read up to 255 characters from the client
    if(n < 0)                         // If reading fails
        error("Error reading from socket");

    // Print the received message to the console
    printf("Here is the message: %s\n", buffer);

    // Send a response message back to the client
    n = write(newsockfd, "I got your message", 18); // Write a response to the client
    if(n < 0)                                     // If writing fails
        error("Error writing to socket");

    return 0; // Exit the program successfully
}
