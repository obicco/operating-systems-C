#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>
#include <ctype.h>

#define N 128
#define NAME "tmp.txt"
#define LOG "log.txt"

static void signalHandler ();
static void child1 ();
static void child2 (pid_t);

int main (void) {
    int pid1, pid2, status, cpid;

    signal(SIGUSR1, signalHandler);

    pid1 = fork();

    if (pid1 == 0)
        child1 ();
    else {
        pid2 = fork();
        if (pid2 == 0)
            child2 (pid1);
    }
    fprintf (stdout, "Sender   PID %d\n", pid2);
    fprintf (stdout, "Receiver PID %d\n", pid1);

    cpid = wait(&status);
    cpid = wait(&status);

    return (0);
}

static void signalHandler(void) {
    return;
}

static void child2 (pid_t pid1) {
    FILE *fp, *fp_log = fopen (LOG, "w");
    char line[N];
    int sent = 0;

    fprintf (fp_log, "%d", getpid());
    fclose(fp_log);
    kill(pid1, SIGUSR1);

    while (1) {
        fprintf(stdout, "SENDER (receiverPid=%d): ", pid1);
        fflush(stdout);
        scanf("%s", line);

        fp = fopen (NAME, "w");
        if (fp == NULL) {
        fprintf(stderr, "Errore open file: %s\n", NAME);
        exit(1);
        }
        fprintf(fp, "%s", line);
        fclose(fp);
        sent++;
        kill(pid1, SIGUSR1);
        if (strcmp(line, "end") == 0)
            break;

        pause();
    }

    exit(sent);
}

static void child1 (void) {
    FILE *fp, *fp_log;
    char line[N];
    int received = 0, sender_pid;
    pause();
    fp_log = fopen (LOG, "r");
    if (fp_log == NULL) {
        fprintf(stderr, "Errore open file: %s\n", LOG);
        exit(1);
    }
    fscanf(fp_log, "%d", &sender_pid);
    fclose(fp_log);

    while (1) {
        pause();
        fp = fopen(NAME, "r");
        if (fp == NULL) {
            fprintf(stderr, "Errore open file: %s\n", NAME);
            exit(1);
        }
        fscanf(fp, "%s", line);
        fclose(fp);

        if (strcmp(line, "end") == 0)
            break;

        for (int i = 0; i < strlen(line); i++) line[i] = toupper(line[i]);
        fprintf (stdout, "RECEIVER (senderPid = %d): %s\n", sender_pid, line);
        fflush (stdout);
        sleep(1);
        received++;     
        kill (sender_pid, SIGUSR1);
    }
    exit(received);
}