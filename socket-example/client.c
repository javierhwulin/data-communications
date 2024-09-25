#include <stdio.h>          // Standard I/O functions
#include <sys/types.h>      // Data types used in system calls
#include <netinet/in.h>     // Structures for internet domain addresses
#include <netdb.h>          // Definition for network database operations

// Function to print an error message and exit the program
void error(char *msg){
    perror(msg);  // Print the error message from the system call
    exit(0);      // Exit the program with a zero status
}

int main(int argc, char *argv[]){
    int sockfd, portno, n;                  // Socket file descriptor, port number, and byte count
    struct sockaddr_in serv_addr;           // Structure for server address
    struct hostent *server;                 // Pointer to host information
    char buffer[256];                       // Buffer to hold the message

    // Check if the required arguments (hostname and port) are provided
    if(argc < 3){
        fprintf(stderr, "usage %s hostname port\n", argv[0]); // Print usage message
        exit(0);  // Exit if not enough arguments are provided
    }

    portno = atoi(argv[2]);  // Convert the provided port number from string to integer

    // Create a socket:
    // AF_INET -> IPv4, SOCK_STREAM -> TCP, 0 -> default protocol (TCP)
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)            // Check if the socket creation failed
        error("ERROR opening socket");

    // Get the server's information by hostname
    server = gethostbyname(argv[1]);
    if(server == NULL){       // If the hostname cannot be resolved
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);  // Exit if the server is not found
    }

    // Initialize the server address structure
    bzero((char *) &serv_addr, sizeof(serv_addr));  // Zero out the server address structure
    serv_addr.sin_family = AF_INET;  // Set the address family to IPv4

    // Copy the server's IP address from the hostent structure to the server address structure
    bcopy((char *)server->h_addr_list[0], (char *)&serv_addr.sin_addr.s_addr, server->h_length);

    serv_addr.sin_port = htons(portno);  // Convert the port number to network byte order

    // Establish a connection to the server
    if(connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR connecting");  // Error if connection fails

    // Prompt the user to enter a message
    printf("Please enter the message: ");
    bzero(buffer, 256);  // Clear the buffer before reading input
    fgets(buffer, 256, stdin);  // Read the input from the user

    // Write the message to the server
    n = write(sockfd, buffer, strlen(buffer));
    if(n < 0)  // Error if writing to the socket fails
        error("ERROR writing to socket");

    // Clear the buffer to prepare for receiving the server's response
    bzero(buffer, 256);

    // Read the response from the server
    n = read(sockfd, buffer, 255);
    if(n < 0)  // Error if reading from the socket fails
        error("ERROR reading from socket");

    // Print the server's response to the console
    printf("%s\n", buffer);

    return 0;  // Exit the program successfully
}

