#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

typedef struct monomial_s {
  float c, x;
  int e;
} monomial_t;

int main (int argc, char **argv) {
  int i, n, fd;
  FILE *fp;
  monomial_t m;
  
  if (argc != 3) {
    fprintf (stderr, "usage: %s fileName\n", argv[0]);
    exit (-1);
  }

  /*
   * Frm ASCII to BIN
   */

  fp = fopen (argv[1], "r");
  fd = creat (argv[2], 0666);
  if (fp==NULL || fd==-1) {
    fprintf(stderr, "Cannot open/create file.\n");
    exit (1); 
  }

  while (fscanf (fp, "%f%f%d", &m.c, &m.x, &m.e) != EOF) {
    write (fd, &m, sizeof (monomial_t));
  }

  fclose (fp);
  close (fd);

  /*
   * Read BIN File
   */

  fd = open (argv[2], O_RDONLY);
  if (fd == -1){
    fprintf(stderr, "Cannot open file %s\n", argv[1]);
    exit(-1);
  }

  fprintf (stdout, "Binary file:\n");
  i = 0;
  do {
    lseek (fd, i*sizeof (monomial_t), SEEK_SET);
    n = read (fd, &m, sizeof (monomial_t));
    if (n > 0){
      fprintf (stdout, "Record=%2d: %.2f %.2f %d\n", i, m.c, m.x, m.e);
    }
    i++;
  } while (n>0);

  close (fd);

  return (0);
}
