# include <stdio.h>

int main() {

  int iUid;
  iUid = fork();
  if (iUid == 0) {
    for (;;) {
      printf("This is child process.\n");
      sleep(1);
    }
  }

  if (iUid > 0) {
    for (;;) {
      printf("This is parent process.\n");
      sleep(1);
    }
  }
  
  if (iUid < 0) {
    printf("Can not use system call.\n");
  }

  return 0;
}
