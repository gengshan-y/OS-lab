#include <stdio.h>
#include <unistd.h>  // fork(), NULL pointer, etc., should be included anyway
#include <stdlib.h>  // exit()
#include <signal.h>

int wait_main;
int wait_flag;
void contMain();
void stop();

int main() {
  int pid1, pid2;
  wait_flag = 1;
  wait_main = 1;
  while((pid1 = fork()) == -1);  // create child proces 1
  if (pid1 > 0) {
    while ((pid2 = fork()) == -1);  // create child proces 2
    if (pid2 > 0) {
      signal(10, contMain);  // can be triggered through bash
      printf("main thread waiting for interruption\n");
      while(wait_main);  // wait for signal to execute contMain
      kill(pid1, 16);  // kill proc 1 and send signal 16
      kill(pid2, 17);  // kill proc 2 and send signal 17
      wait(0);  // wait for child process to exit
      wait(0);
      printf("\nParent process is killed\n");
      exit(0);
    }
    else {
      signal(17, stop);  // wait for signal 17 to execute function 'stop'
      wait_flag = 1;
      printf("Child process 2 is waiting for signal\n");
      while(wait_flag);
      printf("\nChild process 2 is killed by parent\n");
      exit(0);
    }
  }
  else {
    signal(16, stop);
    wait_flag = 1;
      printf("Child process 1 is waiting for signal\n");
    while(wait_flag);
    printf("\nChild process 1 is killed by parent\n");
    exit(0);
  }
}

void stop(int signum) {
  printf("\n%d received, child process stopped\n", signum);
  wait_flag = 0;
}

void contMain() {
  wait_main = 0;
  printf("\nmain continues executing\n");
}
