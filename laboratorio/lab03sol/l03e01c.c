/*
 * Illustrates system call fork
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int
main (
  int argc,
  char *argv[]
  )
{
  int i;

  for (i=0; i<2; i++) {
    if (fork())
      fork ();
  }
  printf ("%d\n", i);
  return (0);
}
