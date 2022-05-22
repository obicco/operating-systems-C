/*
* Sequential Version !
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#define N 12

int
main (
  int argc,
  char **argv
  )
{
  int i, j, v, v1, v2;
  int fd, c, c1, c2;
  int vet[N] = {0, 10, 5, 20, 15, 30, 25, 40, 35, 50, 45, 60};

  if (argc != 2) {
    fprintf (stderr, "usage: %s fileName\n", argv[0]);
    exit (-1);
  }

  /*
   * Write File
   */

  fd = creat (argv[1], 0666);
  if (fd == -1) {
    fprintf(stderr, "Cannot create file %s\n", argv[1]);
    exit (-1); 
  }

  fprintf (stdout, "Data written in the file: ");
  for (i=0; i<N; i++) {
    write (fd, &vet[i], sizeof (int));
    fprintf (stdout, "%2d ", vet[i]);
  }
  fprintf (stdout, "\n");

  close (fd);

  /*
   * Read File
   */

  fd = open (argv[1], O_RDONLY);
  if (fd == -1){
    fprintf(stderr, "Cannot open file %s\n", argv[1]);
    exit(-1);
  }

  fprintf (stdout, "File before ordering    : ");
  for (i=0; i<N; i++) {
    lseek (fd, i*sizeof (int), SEEK_SET);
    c = read (fd, &v, sizeof (int));
    if (c <= 0){
      fprintf (stderr, "Error reading file\n");
      exit (-1);
    }
    fprintf (stdout, "%2d ", v);
  }
  fprintf (stdout, "\n");

  close (fd);

  /*
   *  Ordering
   */

  fd = open (argv[1], O_RDWR);
  if (fd == -1){
    fprintf(stderr, "Cannot open file %s\n", argv[1]);
    exit(-1);
  }

  fprintf (stdout, "SEQUENTIAL ordering ...\n");
  for (i=0; i<N-1; i++) {
    for (j=0; j<N-1-i; j++) {
      lseek (fd, j*sizeof (int), SEEK_SET);
      c1 = read (fd, &v1, sizeof (int));
      //lseek (fd, (j+1)*sizeof (int), SEEK_SET);
      c2 = read (fd, &v2, sizeof (int));
      if (c1<=0 || c2<=0){
        fprintf (stderr, "Error reading file\n");
        exit (-1);
      }
      if (v1 > v2) {
        lseek (fd, j*sizeof (int), SEEK_SET);
        c1 = write (fd, &v2, sizeof (int));
        //lseek (fd, (j+1)*sizeof (int), SEEK_SET);
        c2 = write (fd, &v1, sizeof (int));
      }
    }
  }

  close (fd);

  /*
   * Read File
   */

  fd = open (argv[1], O_RDONLY);
  if (fd == -1){
    fprintf(stderr, "Cannot open file %s\n", argv[1]);
    exit(-1);
  }

  fprintf (stdout, "File after  ordering    : ");
  for (i=0; i<N; i++) {
    lseek (fd, i*sizeof (int), SEEK_SET);
    c = read (fd, &v, sizeof (int));
    if (c <= 0){
      fprintf (stderr, "Error reading file\n");
      exit (-1);
    }
    fprintf (stdout, "%2d ", v);
  }
  fprintf (stdout, "\n");

  close (fd);

 return (0);
}
