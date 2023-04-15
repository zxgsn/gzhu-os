#include<unistd.h>
#include<stdio.h>
#include <stdlib.h>
int main() {
  fork();
  fork();
  printf("pid = %d, ppid = %d\n", getpid(), getppid());
  exit(0);
}