#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#define N 132

void sender (pid_t, int []);
void receiver (int []);

int
main (int argc, char **argv) {
  int fd[2];
  int cpid, status;
  int receiverPid, senderPid;

  pipe (fd);

  receiverPid = fork();
  if (receiverPid == 0) {
    receiver (fd);
  } else {
    senderPid = fork();
    if (senderPid == 0) {
      sender (receiverPid, fd);
    }
  }

  fprintf (stdout, "Sender   PID %d\n", senderPid);
  fprintf (stdout, "Receiver PID %d\n", receiverPid);

  cpid = wait(&status);
  cpid = wait(&status);

  return (0);
}

void sender (pid_t receiverPid, int fd[2]) {
  char line[N], tmp[N];
  int sentMsg=0, senderPid;

  close (fd[0]);
  senderPid = getpid();

  while (1) {
    fprintf (stdout, "SENDER (receiverPid=%d): ",
      receiverPid);
    fflush (stdout);
    scanf ("%s", line);

    sprintf (tmp, "%08d%s", senderPid, line);
    write (fd[1], tmp, N);

    sentMsg++; 	
    if (strcmp (line, "end") == 0)
      break;

    sleep (1);
  }

  exit(sentMsg); 
}

void receiver (int fd[2]) {
  char line[N], tmp[N];
  int receivedMsg=0, senderPid, i;
  
  close (fd[1]);

  while (1) {
    read (fd[0], tmp, N);
    sscanf (tmp, "%8d%s", &senderPid, line);
    if (strcmp(line, "end") == 0)
      break;
              
    for (i=0; i<strlen (line); i++) {
      line[i] = toupper (line[i]);
    }
    fprintf (stdout, "RECEIVER (senderPid=%d): %s\n",
      senderPid, line);
    fflush (stdout);
    receivedMsg++;     
  }  

  exit (receivedMsg); 
}


