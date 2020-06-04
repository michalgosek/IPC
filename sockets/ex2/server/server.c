#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#define SERVER_PORT 1096

int servinit();
int accept_conns(int);
void servend(int);
void send_response(int);
void get_response(int);

int main()
{
    int serverSocket = servinit();
    //Client TCP socket:
    int clientSocket = accept_conns(serverSocket);
    get_response(clientSocket);

    //Sending response to cli:
    send_response(clientSocket);
    servend(serverSocket);
}

int servinit()
{
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        perror("server: socket descriptor creation failed.");
        exit(EXIT_FAILURE);
    }
    else {
        puts("server: server socket descriptor has been created successfully.");
    }

    // set-up the server:
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(SERVER_PORT);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1) {
        perror("server: bidning error during server setup.");
        exit(EXIT_FAILURE);
    } else {
        printf("server: running on port: %d\n", SERVER_PORT);
    }

    listen(serverSocket, 5);
    return serverSocket;
}

void servend(int serverSocket)
{
    if (close(serverSocket) == -1) {
        perror("server: socket has not been closed - ");
        exit(EXIT_FAILURE);
    }
    else {
        puts("server: socket has been closed.");
    }
}

int accept_conns(int serverSocket)
{
    int clientSocket = accept(serverSocket, NULL, NULL);
    if (clientSocket == -1) {
        perror("server: cannot accept client connection:");
        exit(EXIT_FAILURE);
    } else {
        puts("server: client connection accepted");
    }
    return clientSocket;
}

void get_response(int clientSocket) {
    char buffer[128];
    int recivedBytes = recv(clientSocket, &buffer, sizeof(buffer), 0);
    if (recivedBytes == -1) {
        perror("recv:");
        exit(EXIT_FAILURE);
    } else {
        buffer[recivedBytes] = '\0';
        puts("server: response from the client:");
        puts(buffer);
    }
}

void send_response(int clientSocket)
{
    //sending msg to Client:
    const char *msg = "Server confirms reciving the msg from the client!";
    send(clientSocket, msg, strlen(msg), 0);
}



