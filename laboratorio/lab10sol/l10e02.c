#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "pthread.h"
#include "semaphore.h"
#include <sys/timeb.h>

// Number of monomials / threads
#define NT 3

typedef struct monomial_s {
  float c, x;
  int e;
} monomial_t;

typedef struct thread_data_s {
  pthread_t threadId;
  int id;
  int n;
  char *name;
  int stop;
  float result;
  sem_t sem;  
} thread_data_t;

// Global Variable
// Can be passed as parameters to make program more modular
thread_data_t td[NT];

static void *monomial (void *);
static void *polynomial (void *);

int main (int argc, char **argv) {
  int i;

  if (argc != 2) {
    fprintf (stderr, "Syntax: %s fileName\n", argv[0]);
    return (1);
  }
    
  setbuf (stdout, 0);

  td[0].id = 0;
  td[0].n = NT;
  td[0].name = NULL;
  td[0].stop = 0;
  td[0].result = 0;
  sem_init (&td[0].sem, 0, 0);    
  pthread_create (&(td[0].threadId), NULL, polynomial, (void *) &td[0].id);

  for (i=1; i<=NT; i++) {
    td[i].id = i;
    td[i].n = 0;
    td[i].name = argv[1];
    td[i].stop = 0;
    td[i].result = 0;
    sem_init (&td[i].sem, 0, 1);    
    pthread_create (&(td[i].threadId), NULL, monomial, (void *) &td[i].id);
  }

  for (i=0; i<=NT; i++) {
    pthread_join (td[i].threadId, NULL);
  }

  return (1);
}

/*
 * Each monomial thread awaits on its semaphore and
 * awakes polinomial thread on its semaphore
 */
static void *monomial (void *arg) {
  monomial_t m;
  int *idp, id, fd;
  int i, j, n, offset;

  idp = (int *) arg;
  id = *idp;
  
  fd = open (td[id].name, O_RDONLY);
  if (fd == -1){
    fprintf(stderr, "Cannot open file %s\n", td->name);
    exit(-1);
  }

  i = 0;
  do {
    sem_wait (&td[id].sem);

    offset = (i * 3 + td[id].id - 1) * sizeof (monomial_t);
    lseek (fd, offset, SEEK_SET);
    n = read (fd, &m, sizeof (monomial_t));
    if (n > 0){
      td[id].result = m.c;
      for (j=0; j<m.e; j++) {
        td[id].result = td[id].result * m.x;
      }
      fprintf (stdout,
	"Thread=%2d ReadRecord=%2d ReadData=%.2f-%.2f-%2d Result=%.2f\n",
       td[id].id, i+1, m.c, m.x, m.e, td[id].result);
      td[id].stop = 0;
    } else {
      td[id].result = 0;
      td[id].stop = 1;
    }
    i++;

    sem_post (&td[0].sem);
  } while (n>0);

  close (fd);
  
  fprintf (stdout, "Thread=%2d Exit\n", id);
  pthread_exit (0);
}

/*
 * Polinomial thread awaits on its semaphore NT times,
 * and awakes all monomial threads on their own
 * semaphores
 */
static void *polynomial (void *arg) {
  int *idp, id, i, stop;
  float result;

  idp = (int *) arg;
  id = *idp;
  
  do {
    for (i=0; i<td[id].n; i++) {
      sem_wait (&td[id].sem);
    } 

   result = 0;
   stop = 0;
   for (i=1; i<=td[id].n; i++) {
     if (td[i].stop == 1) {
       stop++;
     } else {
       result = result + td[i].result;
     }
   }

   if (stop == td[0].n) {
     pthread_exit (0);
   } else {
     fprintf (stdout, "Thread=%2d Result=%.2f\n", td[id].id, result);
     for (i=1; i<=td[0].n; i++) {
       sem_post (&td[i].sem);
     }
    }
  } while (stop == 0);

  fprintf (stdout, "Thread=%2d Exit\n", id);
  pthread_exit (0);
}
