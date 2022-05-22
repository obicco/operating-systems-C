#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <semaphore.h>
#include <pthread.h>

#define L (100+1+1)

static void *readFile ();
static void *convert ();
static void *writeFile ();

/*
 *  Global variables
 *  Avoid to use an array of structures (one for each trhead)
 */
sem_t sem1, sem2, sem3;
FILE *fpR, *fpW;
char line[L];

int main (int argc, char *argv[]) {
  int i;
  pthread_t tid1, tid2, tid3;

  fprintf (stdout, "Command line: ");
  for (i=0; i<argc; i++) {
    fprintf (stdout, "%s ", argv[i]);
  }
  fprintf (stdout, "\n");

  fpR = fopen (argv[1], "r");
  fpW = fopen (argv[2], "w");
  if (fpR==NULL || fpW==NULL) {
    fprintf (stderr, "Error Opening Output File.\n");
    exit (1);
  }

  sem_init (&sem1, 0, 1);
  sem_init (&sem2, 0, 0);
  sem_init (&sem3, 0, 0);

  pthread_create (&tid1, NULL, readFile, NULL);
  pthread_create (&tid2, NULL, convert, NULL);
  pthread_create (&tid3, NULL, writeFile, NULL);

  pthread_join (tid1, NULL);
  pthread_join (tid2, NULL);
  pthread_join (tid3, NULL);
  fprintf (stdout, "Program end\n");

  fclose (fpR);
  fclose (fpW);

  return (1);
}

static void *readFile () {
  int goOn;
  
  fprintf (stdout, ">>> Read Thread Start\n");
  goOn = 1;
  do {
    sem_wait (&sem1);
    if (fgets (line, L, fpR) == NULL) {
      strcpy (line, "quit");
      goOn = 0;
    } else {
      fprintf (stdout, "Reading = %s", line);
    }
    sem_post (&sem2);
  } while (goOn==1);

  fprintf (stdout, ">>> Read Thread Quit\n");
  
  pthread_exit ((void *) 1); 
}

static void *convert () {
  int i, len, goOn;
  char c;
  
  fprintf (stdout, ">>> Convert Thread Start\n");
  goOn = 1;
  do {
    sem_wait (&sem2);
    if (strcmp (line, "quit") == 0) {
      goOn = 0;
    } else {
      fprintf (stdout, "Converting = %s", line);    
      len = strlen (line) - 1;
      for (i=0; i<len; i++) {
        line[i] = toupper(line[i]);
      }
      for (i=0; i<((int) ((len-1)/2)); i++) {
        c = line[i];
        line[i] = line[len-1-i];
        line[len-1-i] = c;
      }
    }
    sem_post (&sem3);
  } while (goOn == 1);

  fprintf (stdout, ">>> Convert Thread Quit\n");
  
  pthread_exit ((void *) 1); 
}

static void *writeFile () {
  int goOn;
  
  fprintf (stdout, ">>> Write Thread Start\n");
  goOn = 1;
  
  do {
    sem_wait (&sem3);
    if (strcmp (line, "quit") == 0) {
      goOn = 0;
    } else {
      fprintf (stdout, "Writing = %s", line);    
      fprintf (fpW, "%s", line);
    }
    sem_post (&sem1);
  } while (goOn == 1);

  fprintf (stdout, ">>> Write Thread Quit\n");
  
  pthread_exit ((void *) 1); 
}
