#include <stdio.h>
#include <unistd.h>
#include <signal.h>

static void signalHandler ();
static void father (pid_t);
static void child (pid_t);

int
main (void) {
  pid_t pid;

  if (signal(SIGUSR1, signalHandler) == SIG_ERR) {
    fprintf (stderr, "Signal Handler Error.\n");
    return (1);
  }

  pid = fork ();

  if (pid==0) {
    // Child Running
    child (getppid ());
  } else {
    // Father Running
    father (pid);
  }

  return (0);
}

static void
signalHandler (
  void
) {
  return;
}

static void
father (
  pid_t pid
) {
  fprintf (stdout, "Father Running: pid=%d, childPid=%d\n",
    getpid (), pid);

  while (1) {
    sleep (2);
    kill (pid, SIGUSR1);
    pause ();
    fprintf (stdout, "Father Woke-up\n");
  }
  return;
}

static void
child (
  pid_t pid
) {
  fprintf (stdout, "Child Running: pid=%d, fatherPid=%d\n",
    getpid (), pid);

  while (1) {
    pause ();
    fprintf (stdout, "Child Woke-up\n");
    sleep (2);
    kill (pid, SIGUSR1);
  }
  return;
}
