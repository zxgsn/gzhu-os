#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define LockStdOut() lockf(1, 1, 0)
#define UnlockStdOut() lockf(1, 0, 0)
#define LockFile() lockf(fd, 1, 0)
#define UnlockFile() lockf(fd, 0, 0)

/*
int sum = 0; 
int main() {

    int pid1 = fork();
    if (pid1 == 0) { // 子进程执行
        LockStdOut();
        printf("pro1 begin\n");
        UnlockStdOut();
        printf("1 begin sum = %d\n", sum);
        for (int i = 0; i < 10; ++i) {
            sum += 1;
        }
        printf("1 begin sum = %d\n", sum);
        exit(0);
    } else { // 父进程再创建一个进程与子进程并发执行
        int pid2 = fork();
        if (pid2 == 0) { // 子进程执行
            LockStdOut();
            printf("pro2 begin\n");
            UnlockStdOut();
            printf("2 begin sum = %d\n", sum);
            for (int i = 0; i < 10; ++i) {
                sum += 1;
            }
            printf("2 end sum = %d\n", sum);
            exit(0);
        } else {  // 父进程执行
            LockStdOut();
            printf("prop begin\n");
            UnlockStdOut();
            printf("p begin sum = %d\n", sum);
            for (int i = 0; i < 10; ++i) {
                sum += 1;
            }
            printf("p end sum = %d\n", sum);
        }
    }
    printf("sum = %d\n", sum);
}*/
/*
int main() {
    FILE* pf = fopen("./test.txt", "a");

    if (pf == NULL) {
        perror("fopen");
        return 1;
    }

    if (fork() == 0) {
        // child process
        for (int i = 0; i < 5; ++i) {
            fputs("a\n", pf);
        }
    } else {
        if (fork() == 0) {
            // 子2
            for (int i = 0; i < 5; ++i) {
                fputs("b\n", pf);
            }
        } else {
            for (int i = 0; i < 5; ++i) {
            // 父进程
                fputs("c\n", pf);
            }
        
        }
    }   
    fclose(pf);
    pf = NULL;
}
*/

#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#define LockStdOut() lockf(1, 1, 0)
#define UnlockStdOut() lockf(1, 0, 0)
#define LockFile() lockf(fd, 1, 0)
#define UnlockFile() lockf(fd, 0, 0)

void child_process(int id, int fd)
{
    int i;
    
    printf("Child process %d start.\n", id);
    //LockStdOut();
    LockFile();
    for (i = 0; i < 5; i++) {
        
        printf("Child process %d write %d\n", id, i);
        sleep(1);
        printf("Child process %d endwrite\n", id);

        sleep(1);
    }
    printf("Child process %d end.\n", id);
    UnlockFile();
    UnlockStdOut();
    exit(0);
}

int main()
{
    int fd, i;
    pid_t pid;
    
    fd = open("test.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (fd == -1) {
        perror("open");
        exit(1);
    }

    for (i = 0; i < 3; i++) {
        pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(1);
        } else if (pid == 0) {
            child_process(i + 1, fd);
        }
    }

    for (i = 0; i < 3; i++) {
        wait(NULL);
    }
    close(fd);

    return 0;
}
