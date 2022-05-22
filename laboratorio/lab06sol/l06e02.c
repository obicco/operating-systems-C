#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#define N 132

void parent (int [],int []);
void child (int [],int []);

int
main (int argc, char **argv) {
  int fd1[2],fd2[2];
  int childPid, parentPid;

  pipe (fd1);
  pipe (fd2);

  childPid = fork();
  if (childPid == 0) {
    child (fd1,fd2);   
  } else {
      parent (fd1,fd2);   
  }
  
  wait((void *)0); 
  exit(0);
}

void  parent(int fd1[2],int fd2[2]) {
  char tmp[N];
  close (fd1[0]); //close read end
  close (fd2[1]); //close write end
  int i=0;
  while (i<10) {
    printf ("PARENT WAKE UP (PID=%d) \n",getpid());
    //sleep(1);  
    sprintf (tmp,"X");
    write (fd1[1], tmp, (strlen(tmp)+1));
    i++; 
    read (fd2[0], tmp, N); 
    
  }

  sprintf (tmp,"end");
  write (fd1[1], tmp, (strlen(tmp)+1)); 
  close(fd1[1]);
  close(fd2[0]);
}

void child (int fd1[2],int fd2[2]) {
  char tmp[N];
  close (fd1[1]); //close write end
  close (fd2[0]); //close read end
  while (1) {    
    read (fd1[0], tmp, N);
    
    if (strcmp(tmp,"end")==0)
    {
     close(fd1[0]);  
     close(fd2[0]);  
     exit(0);  
    }
    else
    {
     printf ("CHILD WAKE UP (PID=%d)\n",getpid());
    }
    //sleep(1);
    sprintf (tmp,"X");
    write (fd2[1], tmp, (strlen(tmp)+1));
     
  }  
}
