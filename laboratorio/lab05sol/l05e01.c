#include <stdio.h>
#include <stdlib.h>

int
main (
  )
{
  int n;

  do {
    scanf ("%d", &n);
    if (n%2==0)
      fprintf (stdout, "PARI %d\n", n);
    else
      fprintf (stderr, "DISPARI %d\n", n);
  } while (n!=0);

  return (0);
}
