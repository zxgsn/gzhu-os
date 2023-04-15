#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {
    int status;
    if (fork() == 0) {
        // 子进程执行
        printf("pid = %d\n", getpid());
        sleep(100);
        // execlp("/bin/pstree", "pstree", "-p", NULL);
        execlp("/bin/ls", "ls", NULL);
    }
    wait(&status);
}