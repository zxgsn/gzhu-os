#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {
    int status;
    if (fork() == 0) {
        // 子进程1执行
        // sleep(10);
        printf("this is child process1\n");
        exit(1);
        
    }
    if (fork() == 0) {
        // 子进程2执行 
        sleep(1);
        printf("this is child process2\n");
        
        exit(1);
    }
    wait(&status);
    printf("this is parent process\n");
    // exit(0);
}