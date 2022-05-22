/*
 * Precedence graph with fork and wait.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

pid_t pLeft (void);
void pRight (void);

int
main()
{
  pid_t pid, pidTmp;

  fprintf (stdout, "P1 (pid=%d)\n", getpid());
  pid = fork();
  if (pid == 0) {
    //sleep (1);
    pRight ();
    exit (0);
  } else {
    //sleep (1);
    pidTmp = pLeft ();
    // Check whether pLeft got the exit from P8
    if (pidTmp!=pid) {
       // Exit from P8 still to catch
       pidTmp = wait ((int *) 0);
       fprintf (stdout, "pid=%d return from wait pid=%d\n",
         getpid(), pidTmp);
    }
    fprintf (stdout, "P9 (pid=%d)\n", getpid());
    return (0);
  }
}

pid_t
pLeft () {
  pid_t pid, pidRet, pidTmp;

  fprintf (stdout, "P2 (pid=%d)\n", getpid());
  pid = fork();
  if (pid == 0) {
    //sleep (1);
    fprintf (stdout, "P5 (pid=%d)\n", getpid());
    exit (0);
  } else {
    fprintf (stdout, "P4 (pid=%d)\n", getpid());
    //sleep (1);
    // Wait once
    pidRet = wait ((int *) 0);
    fprintf (stdout, "pid=%d return from wait pid=%d\n",
      getpid(), pidRet);
    // IFF P5 go on, IFF P8 wait again 
    if (pidRet != pid) {
      pidTmp = wait ((int *) 0);
      fprintf (stdout, "pid=%d return from wait pid=%d\n",
        getpid(), pidTmp);
    } 
    fprintf (stdout, "P7 (pid=%d)\n", getpid());
    return (pidRet);
  }
}

void
pRight () {
  pid_t pid, pidTmp;

  //sleep(1);
  fprintf (stdout, "P3 (pid=%d)\n", getpid());
  pid = fork();
  if (pid == 0) {
    //sleep (1);
    fprintf (stdout, "P6 (pid=%d)\n", getpid());
    exit (0);
  } else {
    //sleep (1);
    pidTmp = wait ((int *) 0);
    fprintf (stdout, "pid=%d return from wait pid=%d\n",
      getpid(), pidTmp);
    fprintf (stdout, "P8 (pid=%d)\n", getpid());
    exit (0);
  }
}
