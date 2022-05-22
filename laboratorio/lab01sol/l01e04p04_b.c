#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFDIM 1024

int
main( int argc, char ** argv)
  {
  char buffer[BUFDIM];
  int oldfd, newfd;
  int count;

  if ( argc != 3 ) {
    fprintf(stderr,"usage: %s oldfile newfile\n", argv[0] );
    exit( -1 );  // return -1;
  }

  /* open source file in read-only mode */
  oldfd = open (argv[1], O_RDONLY);
  if (oldfd == -1){
    fprintf(stderr,"%s: cannot open file %s\n", argv[0], argv[1] );
    exit(-1);
  }

  /* create dest. file in read-write mode */
  newfd = creat (argv[2], 0666);
  if (newfd == -1) {
    fprintf(stderr, "%s: cannot create file %s\n",
      argv[0], argv[2]);
    exit (-1); 
  }

  while ((count = read( oldfd, buffer, BUFDIM)) > 0) {
    write (newfd, buffer, count);
  }

  close (newfd);
  close (oldfd);

  return (0);
}
