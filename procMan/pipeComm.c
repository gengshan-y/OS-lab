#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

int pid1, pid2;

main() {
  int fd[2];  // file descripter for piping, 0 for reading and 1 for writing
  char outPipe[100], inPipe[100];
  pipe(fd);
  while ((pid1 = fork()) == -1);
  if (pid1 == 0) {
    lockf(fd[1], 1, 0);  // lock writing pipe to end-of-file
    sprintf(outPipe, "\nChild process 1 is sending message\n");
    write(fd[1], outPipe, 50);
    sleep(2);
    lockf(fd[1], 0, 0);  // unlock
    exit(0);
  }
  else {
    while ((pid2 = fork()) == -1);
    if (pid2 == 0) {
      lockf(fd[1], 1, 0);
      sprintf(outPipe, "\nChild process 2 is sending message");
      write(fd[1], outPipe, 50);
      sleep(2);
      lockf(fd[1], 0, 0);
      exit(0);
    }
    else {
      wait(0);
      read(fd[0], inPipe, 50);
      printf("%s\n", inPipe);
      wait(0);
      read(fd[0], inPipe, 50);
      printf("%s\n", inPipe);
      exit(0);
    }
  }
}
