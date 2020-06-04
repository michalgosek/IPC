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
    // client socket creation
    int descriptor = socket(AF_INET, SOCK_DGRAM, 0);
    if (descriptor == - 1) {
        perror("client: cannot create socket descriptor: ");
        return 1;
    } else {
        puts("client: socket descriptor has been created.");
    }

    // basic set-up
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = PORT;
    serverAddress.sin_addr.s_addr = htons(INADDR_ANY); //localhost
    
    const char *msg = "Hello from client! :)";
    if (sendto(descriptor, msg, strlen(msg), 0, (sockaddr *)&serverAddress, sizeof(serverAddress)) == - 1) {
        perror("client: cannot send messege to the server :");
        return 1;
    } else {
        printf("client: send msg %s to the server.\n", msg);
    }

    // getting response from the server 
    char buffer[128];
    socklen_t addrLen;
    int response = recvfrom(descriptor, (void*)buffer, sizeof(buffer), 0, (sockaddr*)&serverAddress, &addrLen);
    if (response == - 1) {
        perror("client: cannot recieve messege from server:");
        return 1; 
    } else {
        puts("client: response from the server:");
        puts(buffer);
    }

    // Closing client socekt 
    if (close(descriptor) == - 1) {
        perror("client: cannot close socket:");
        return 1;
    } else {
        puts("client: socket has been closed successfuly.");
    }

    return 0;
}