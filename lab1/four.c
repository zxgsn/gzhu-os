#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>


int Gpid1, Gpid2;
int two_or_not = 1;

void HandleSignalForParentProcess(int sig) {
    kill(Gpid1, sig); 
    kill(Gpid2, sig);
    waitpid(Gpid1, NULL, 0);
    waitpid(Gpid2, NULL, 0);
    printf("Parent process is killed!\n");
    exit(0);
}

void HandleSignalForChildentProcess(int sig) {
    printf("Child process %d is killed by parent!\n", two_or_not + 1);
    exit(0);
}

int main() {
  pid_t pid1, pid2;
  if ((pid1 = fork()) < 0) {
    printf("fork error\n");
    exit(1);
  } else if (pid1 == 0) { // 子进程1运行
    two_or_not = 0;
    Gpid1 = getpid();
    printf("this is child process 1, pid is %d\n", getpid());
    signal(SIGINT, HandleSignalForChildentProcess); // 接受中断信号
    while(1) {};
    exit(0);
  } else {
    if ((pid2 = fork()) < 0) {
      printf("fork error\n");
      exit(1);
    } else if (pid2 == 0) {
    Gpid2 = getpid();
    // 子进程2运行
    // sleep(120);
    printf("this is child process 2, pid is %d\n", getpid());
    signal(SIGINT, HandleSignalForChildentProcess); // 接受中断信号
    while(1) {};
    exit(0);
  } else {
      // 父进程的任务
      printf("this is parent process, pid is %d\n", getpid());
      
      signal(SIGINT, HandleSignalForParentProcess); // 接受中断信号
      while(1) {}
      // 等待两个子进程完成
      printf("both processes has finished\n");
      exit(0);
    }
  }
}