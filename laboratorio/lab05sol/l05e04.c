#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#define N 132
#define NAME "tmp.txt"

void sender (pid_t);
void receiver ();
void catcher ();

int
main (int argc, char **argv) {
  int cpid, status;
  int receiverPid, senderPid;

  signal(SIGUSR1,catcher);

  receiverPid = fork();
  if (receiverPid == 0) {
    receiver ();
  } else {
    senderPid = fork();
    if (senderPid == 0) {
      sender (receiverPid);
    }
  }

  fprintf (stdout, "Sender   PID %d\n", senderPid);
  fprintf (stdout, "Receiver PID %d\n", receiverPid);

  cpid = wait(&status);
  cpid = wait(&status);

  return (0);
}

void
catcher() {
  //fprintf (stdout, "CATCHER\n");
  //fflush (stdout);
  return;
}

void sender (pid_t receiverPid) {
  FILE *fp;
  char line[N];
  int sentMsg=0, senderPid;

  senderPid = getpid();

  while (1) {
    fprintf (stdout, "SENDER (receiverPid=%d): ", receiverPid);
    fflush (stdout);
    scanf ("%s", line);

    fp = fopen (NAME, "w");
    if (fp == NULL) {
      fprintf(stderr, "Errore open file: %s\n", NAME);
      exit(1);
    }
    fprintf (fp, "%d %s", senderPid, line);
    fclose(fp);

    sentMsg++; 	
    kill (receiverPid, SIGUSR1);
    if (strcmp(line, "end") == 0)
      break;

    pause ();
  }

  exit(sentMsg); 
}

void receiver () {
  FILE *fp;
  char line[N];
  int receivedMsg=0, senderPid, i;
  
  while (1) {
    pause();

    fp = fopen (NAME, "r");
    if (fp == NULL) {
      fprintf(stderr, "Errore open file: %s\n", NAME);
      exit(1);
    }
    fscanf (fp, "%d %s", &senderPid, line);
    fclose(fp);
    
    if (strcmp(line, "end") == 0)
      break;
              
    for (i=0; i<strlen(line); i++)
      line[i] = toupper(line[i]);
    fprintf (stdout, "RECEIVER (senderPid=%d): %s\n", senderPid, line);
    fflush (stdout);
    sleep(1);
    receivedMsg++;     
    kill (senderPid, SIGUSR1);
  }  

  exit (receivedMsg); 
}


