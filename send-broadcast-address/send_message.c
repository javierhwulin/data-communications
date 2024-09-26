#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error(char *msg){
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]){
    int sockedfd;
    struct sockaddr_in broadcast_addr;
    
}