#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>

#define PORT 2020

// alisaes
typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;

int main(){
    int descriptor = socket(AF_INET, SOCK_DGRAM, 0);
    if (descriptor == - 1) {
        perror("server: cannot create socket descriptor.");
        exit(EXIT_FAILURE);;
    } else {
        puts("server: socket descriptor has been created.");
    }

    // basic set-up
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = PORT;
    serverAddress.sin_addr.s_addr = htons(INADDR_ANY); //localhost
 
    // binding socket descriptor with server configuration
    if (bind(descriptor, (sockaddr *)&serverAddress, sizeof(serverAddress)) == -1) {
        perror("server: cannot bind descriptor with server configuration :");
        exit(EXIT_FAILURE);;
    } else {
        puts("server: binding descriptor with server configuration has been successful.");
    }

    // client
    char buffer[128];
    sockaddr_in clientAddres;
    clientAddres.sin_family = AF_INET;
    clientAddres.sin_port = PORT;
    socklen_t addrLen = sizeof(clientAddres);

    // getting response from the client 
    int response = recvfrom(descriptor, (void*)buffer, sizeof(buffer), 0, (sockaddr*)&clientAddres, &addrLen);
    if (response == - 1) {
        perror("server: cannot recieve messege from client:");
        exit(EXIT_FAILURE);; 
    } else {
        puts("server: messege from the client:");
        puts(buffer);
    }

    // sending response to the client
    const char *serverResponse = "server response: thank you for your msg! :)";
    if (sendto(descriptor, serverResponse, strlen(serverResponse), 0, (sockaddr*)&clientAddres, addrLen) == - 1) {
        perror("server: cannot send response to the client:");
        exit(EXIT_FAILURE);; 
    } else {
        puts("server: response has been sent to the client.");
        printf("response msg: %s\n", serverResponse);
    }

    // Closing server socekt 
    if (close(descriptor) == - 1) {
        perror("server: cannot close socket:");
        exit(EXIT_FAILURE);;
    } else {
        puts("server: socket has been closed successfuly.");
    }
    return EXIT_SUCCESS;
}