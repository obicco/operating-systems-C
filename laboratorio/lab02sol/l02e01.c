/*
 *  StQ 07.04.2011 & 12.05.2012
 *  Read Dir Recursive
 *
 */

#include  <sys/types.h>
#include  <sys/stat.h>
#include  <dirent.h>
#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>
#include  <unistd.h>

#include  <limits.h>
#include  <errno.h>
#include  <stdarg.h>

#define N 100
#define SPACE 2

void visitDirRecur (char *, int);
void myPrint (int);

int main (
  int argc,
  char *argv[]
  )
{
  if (argc != 2) {
    fprintf (stderr, "Usage: pgrm <pathname>");
    exit (1);
  }

  visitDirRecur (argv[1], 1);

  return (0);
}

void
visitDirRecur (
  char *fullnameR,
  int level
  )
{
  DIR *dp;
  struct stat statbuf;
  struct dirent *dirp;
  char nameR[N];

  myPrint (level);
  fprintf (stdout, "Entering %s; level %d\n", fullnameR, level);

  if (lstat(fullnameR, &statbuf) < 0 ) {
    fprintf (stderr, "Error Running lstat.\n");
    exit (1);
  }

  /* File */
  if (S_ISDIR(statbuf.st_mode) == 0) {
    return (1);
  }

  /* Directory */
  if ( (dp = opendir(fullnameR)) == NULL) {
    fprintf (stderr, "Error Opening Dir.\n");
    exit (1);
  }

  while ( (dirp = readdir(dp)) != NULL) {
    sprintf (nameR, "%s/%s", fullnameR, dirp->d_name);

    if (lstat(nameR, &statbuf) < 0 ) {
      fprintf (stderr, "Error Running lstat.\n");
      exit (1);
    }

    if (S_ISDIR(statbuf.st_mode) == 0) {
      /* File */
      myPrint (level+1);
      fprintf (stdout, "Reading %s; level %d\n", dirp->d_name, level+1);
    } else {
      /* Directory */
      if (strcmp(dirp->d_name, ".") == 0 ||
        strcmp(dirp->d_name, "..") == 0)
        continue;

      visitDirRecur (nameR, level+1);
    }
  }

  if (closedir(dp) < 0) {
    fprintf (stderr, "Error.\n");
    exit (1);
  }

  return;
}

void
myPrint (
  int n
  )
{
  int i;

  for (i=0; i<(n-1)*SPACE; i++) {
    fprintf (stdout, " ");
  }

  return;
}
