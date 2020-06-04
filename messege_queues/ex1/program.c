#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define TEXT_LIMIT 64
struct msgbuf {
    long type;
    char mtext[TEXT_LIMIT];
};

typedef struct msgbuf msgbuf;

int main(){
    int msqid = msgget(IPC_PRIVATE, 0600); //read, write owner
    if (msqid == -1) {
        perror("msget");
        exit(EXIT_FAILURE);
    } 

    printf("program: private msg queue created with key: %d\n", msqid);
    msgbuf sender;
    sender.type = 1;
    const char *msg = "Hello World!";
    strncpy(sender.mtext, msg, strlen(msg));

    // Sending msg into queue
    int status = msgsnd(msqid, &sender, sizeof(sender.mtext), 0);
    if (status == -1) {
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }

    printf("program: %s inserted into the msg queue.\n", msg);

    // receiving msg from the queue
    msgbuf receiver;
    status = msgrcv(msqid, &receiver, sizeof(receiver), 1, 0);
     if (status == -1) {
        perror("msgrcv");
        exit(EXIT_FAILURE);
    }

    puts("program: recived from the msg queue:");
    puts(receiver.mtext);

    return EXIT_SUCCESS;
}