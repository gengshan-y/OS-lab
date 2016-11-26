#include <stdio.h>
// #include <linux/unistd.h>

long newcall(int i) {
  return syscall(380, i);
}

int main(void) {
  printf("%ld\n", newcall(100));
  return 0;
}
