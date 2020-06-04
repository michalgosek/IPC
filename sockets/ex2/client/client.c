#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <arpa/inet.h>
#include <netinet/ip.h>

#define SERVER_PORT 1096

void send_request(int);
void get_response(int);
void make_conn(int);
void close_conn(int);


int main() {
    // setting up Client TCP Socket
    // 0 - default protocol, TCP
    int descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (descriptor == - 1){
        perror("client: socket descriptor creation failed.");
        exit(EXIT_FAILURE);
    }

    make_conn(descriptor);
    send_request(descriptor);
    get_response(descriptor);
    close_conn(descriptor);
    return EXIT_SUCCESS;
}

void close_conn(int descriptor) {
    // closing the connection
    if (close(descriptor) == -1){
        perror("close");
        exit(EXIT_FAILURE);
    };
}
void make_conn(int descriptor) {
    // setting up serverAddress connection
    struct sockaddr_in srvAddr;
    srvAddr.sin_family = AF_INET;
    srvAddr.sin_addr.s_addr = INADDR_ANY;  // localhost
    srvAddr.sin_port = htons(SERVER_PORT); // converting to network byte order

    //Establish connection to the server
    int conStatus = connect(descriptor, (struct sockaddr *)&srvAddr, sizeof(srvAddr));
    if (conStatus < 0) {
        perror("client: connection to the server can not be established - reason:");
        exit(EXIT_FAILURE);
    }
    else {
        puts("client: connection to the server has been established properly.");
    }
}

void send_request(int descriptor) {
    const char *msg = "Hello from the Client :)";
    if (send(descriptor, msg, strlen(msg), 0) < 0)
    {
        perror("client: sending data to the server has been fault:");
        exit(EXIT_FAILURE);
    }
    else {
        printf("client: sent '%s' to the server successfuly.\n", msg);
    }
}

void get_response(int descriptor) {
    // reciving response from the server
    char buffer[128]; // random size
    int recivedBytes = recv(descriptor, buffer, sizeof(buffer), 0);
    if (recivedBytes < 0) {
        perror("recv:");
        exit(EXIT_FAILURE);
    } else {
        buffer[recivedBytes] = '\0';
        puts("client: response from the server:");
        puts(buffer);
    }
}
