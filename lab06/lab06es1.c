#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>
#include <ctype.h>

#define N 256

static void sender (pid_t, int []);
static void receiver (int []);

int main (void) {
    int fd[2], receiverPid, SenderPid, status, cpid;

    pipe(fd);

    receiverPid = fork();

    if (receiverPid == 0)
        receiver (fd);
    else {
        SenderPid = fork();
        if (SenderPid == 0)
            sender (receiverPid, fd);
    }
    fprintf (stdout, "Sender   PID %d\n", SenderPid);
    fprintf (stdout, "Receiver PID %d\n", receiverPid);

    cpid = wait(&status);
    cpid = wait(&status);

    return (0);
}

static void sender (pid_t receiverPid, int fd[2]) {
    char line[N], tmp[N];
    int sent = 0;

    close(fd[0]);

    while (1) {
        fprintf(stdout, "SENDER (receiverPid=%d): ", receiverPid);
        fflush(stdout);
        scanf("%s", line);

        sprintf(tmp, "%08d%s", getpid(), line);
        write(fd[1], tmp, N);
        sent++;
        if (strcmp(line, "end") == 0)
            break;

        sleep(1);
    }

    exit(sent);
}

static void receiver (int fd[2]) {
    char line[N], tmp[N];
    int received = 0, sender_pid;
    close(fd[1]);

    while (1) {
        read (fd[0], tmp, N);
        sscanf(tmp, "%08d%s", &sender_pid, line);
        if (strcmp(line, "end") == 0)
            break;

        for (int i = 0; i < strlen(line); i++) line[i] = toupper(line[i]);
        fprintf (stdout, "RECEIVER (senderPid = %d): %s\n", sender_pid, line);
        fflush (stdout);
        received++;     
    }
    exit(received);
}