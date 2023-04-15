#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>

int main() {
  pid_t pid1, pid2;
  if ((pid1 = fork()) < 0) {
    printf("fork error\n");
    exit(1);
  } else if (pid1 == 0) {
    // 子进程1运行
    // sleep(120);
    printf("this is child process 1, pid is %d\n", getpid());
    exit(0);
  } else {
    if ((pid2 = fork()) < 0) {
      printf("fork error\n");
      exit(1);
    } else if (pid2 == 0) {
    // 子进程2运行
    // sleep(120);
    printf("this is child process 2, pid is %d\n", getpid());
    exit(0);
  } else {
      // 父进程的任务
      printf("this is parent process, pid is %d\n", getpid());
      waitpid(pid1, NULL, 0);
      waitpid(pid2, NULL, 0);
      // 等待两个子进程完成
      printf("both processes has finished\n");
      exit(0);
    }
  }
}