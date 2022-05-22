#include <stdio.h>

int 
main (int argc, char **argv) { 
  FILE * fs, *fd;
  char c;
  char name[1024], buffer[1024];

  // Solution 1
  sprintf (name, "%s.1", argv[2]);
  fs = fopen (argv[1], "r");
  fd = fopen (name, "w");
  while (fscanf(fs,"%c", &c) != EOF) {
    fprintf(fd, "%c", c);
  }
  fclose(fs);
  fclose(fd);

  // Solution 2
  sprintf (name, "%s.2", argv[2]);
  fs = fopen (argv[1], "r");
  fd = fopen (name, "w");
  while ((c = fgetc (fs)) != EOF) {
    fputc (c, fd);
  }
  fclose(fs);
  fclose(fd);
  
  // Solution 3
  sprintf (name, "%s.3", argv[2]);
  fs = fopen (argv[1], "r");
  fd = fopen (name, "w");
  while (fgets (buffer, 132, fs) != NULL) {
    fputs (buffer, fd);
  }
  fclose(fs);
  fclose(fd);

  return (0);
}
