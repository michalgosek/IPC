#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define TEXT_LENGTH 64

struct msgbuf {
    long type;
    char mtext[TEXT_LENGTH];
};

typedef struct msgbuf msgbuf;

void child_worker(int msqid);
void parent_worker(int msqid);

int main(){
    int msqid = msgget(IPC_PRIVATE, 0600);
    if (msqid == - 1){
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid == - 1) {
        perror("pid");
        exit(EXIT_FAILURE);
    }

    printf("program: private msg queue created with key: %d\n", msqid);
    if (pid == 0)
        child_worker(msqid);
    else    
        parent_worker(msqid);

    return EXIT_SUCCESS;
}

void child_worker(int msqid){
    printf("Child[PID:%d]\n", getpid());
    msgbuf receiver;
    int status = msgrcv(msqid, &receiver, sizeof(receiver), 1, 0);
     if (status == -1) {
        perror("msgrcv");
    }

    puts("child process: recived from the msg queue:");
    puts(receiver.mtext);
    exit(0);
}

void parent_worker(int msqid){
    printf("Parent PID: %d\n", getppid());
    msgbuf sender;
    sender.type = 1;
    const char *txt = "Hello from Parent!";

    memset(sender.mtext, 0, sizeof(sender.mtext));
    strncpy(sender.mtext, txt, strlen(txt));

    int status = msgsnd(msqid, &sender, sizeof(sender.mtext), 0);
    if (status == -1) {
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }

    puts("parent process: msg insert into the queue.");

    if (wait(0) == -1) {
        perror("wait");
        exit(EXIT_FAILURE);
    }
}