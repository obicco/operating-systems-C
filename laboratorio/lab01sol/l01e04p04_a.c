#include <stdio.h>

int 
main (int argc, char **argv) { 
  FILE * fs, *fd;
  char buffer[1024];
  int n_elem;

  fs = fopen (argv[1], "rb");
  fd = fopen (argv[2], "wb");
  while ((n_elem = fread (buffer, 1, sizeof (buffer), fs)) != 0) {
    fwrite (buffer, 1, n_elem, fd);
  }
  fclose(fs);
  fclose(fd);

  return (0);
}
