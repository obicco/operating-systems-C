#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/wait.h>

#define OPZIONI 10
#define LEN     10
#define MAX     80

int
main (
  int argc,
  char* argv[]
  )
{
  char tmp[MAX], command[MAX], **mat, *tmpP;
  FILE *fp;
  int i, nString;
  pid_t pid;

  // Alloco a priori ... anche se sasrebbe meglio essere piu' accurati 
  mat = (char **) malloc (OPZIONI * sizeof (char *));
  for (i=0; i<OPZIONI; i++) {
    mat[i] = (char *) malloc (LEN * sizeof (char));
  }

  /*
   *  System Call: System
   */

  fp = fopen(argv[1], "r");
  if (fp==NULL) {
    fprintf (stderr, "Error opening file.");
    exit (1);
  }

  command[0] = '\0';
  while (fscanf(fp, "%s", tmp) != EOF) {
    if (strcmp(tmp,"end") != 0) {
      strcat (command, tmp);
      strcat (command, " ");
    } else {
      printf ("Comando: %s\n", command);
      system (command);
      command[0] = '\0';
    }
  }
  fclose(fp);

  /*
   *  System Call: Exec
   */

  fp = fopen(argv[1], "r");
  if (fp==NULL) {
    fprintf (stderr, "Error opening file.");
    exit (1);
  }

  nString = 0;
  while (fscanf(fp, "%s", tmp) != EOF) {
    if (strcmp(tmp,"end") != 0) {
      strcpy (mat[nString], tmp);
      nString++;
    } else {
      printf ("Comando: ");
      for (i=0; i<nString; i++) {
        printf ("%s ", mat[i]);
      }
      printf ("\n");
      tmpP = mat[nString];
      mat[nString] = NULL;
      pid = fork();
      if (pid==0) {
        execvp (mat[0], mat);
      } else {
        sleep (3);
      }
      mat[nString] = tmpP;
      nString = 0;
    }
  }
  fclose(fp);         

  return (1);
}
