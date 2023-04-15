
/*
#include<stdio.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>


6、编制一长度为1k的共享存储区发送和接收的程序，并设计对该共享存储区进行互斥访问及进程同步的措施，必须保证实现正确的通信。


int shmid,i; int *addr;

void send() {
    int i;
    key_t key = 1234;
    shmid = shmget(key, 1024, 0777); // 打开一个共享存储区
    addr = shmat(shmid, 0, 0); // 共享存储区的附接。从逻辑上将一个共享存储区附接到进程的虚拟地址空间上
    for (i = 9; i >= 0; i--) {
        while (*addr != -1)
            printf("(client) sent a message\n");
        *addr = i;
    }
    exit(0);
}

void receive() {
    key_t key = 1234;
    shmid = shmget(key, 1024, 0777 | IPC_CREAT); // 创建一个共享存储区
    addr = shmat(shmid, 0, 0); // 共享存储区的附接。从逻辑上将一个共享存储区附接到进程的虚拟地址空间上

    do {
        *addr = -1;
        while (*addr == -1)
            printf("(server) receive a message\n");
    } while (*addr);
    shmctl(shmid, IPC_RMID, 0); // 撤销共享内存区，归还资源
    exit(0);
}


int main() {
    while ((i = fork()) == -1); // 创建发送进程
    if(!i) {
        receive();
    }
    system("ipcs -m"); // key = 04d2
    while ((i = fork()) == -1); // 创建接收进程
    if (!i) {
        send();
    }
    wait(0); // 实现同步
    wait(0);
}
*/

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define SHMKEY 1024 // 共享存储区的key
int shmid, i;
int *addr;

void client()
{
    int i;
    shmid = shmget(SHMKEY, 1024, 0777 | IPC_CREAT); /*创建一个共享存储区，名字为75，大小为1024字节，不重复创建*/
    addr = shmat(shmid, 0, 0);                      /*共享存储区所附接到的进程虚地址（首地址）*/
    for (i = 9; i >= 0; i--)
    {
        while (*addr != -1); //确保服务端收到了一条信息，再发下一条
        printf("(client)sent\n");
        *addr = i;
    }
    exit(0);
}

void server()
{
    shmid = shmget(SHMKEY, 1024, 0777 | IPC_CREAT); //创建一个共享存储区,大小为1024字节
    addr = shmat(shmid, 0, 0);                      //共享存储区所附接到的进程虚地址（首地址）
    do
    {
        *addr = -1;
        while (*addr == -1);//响应客户端
        printf("(server)received\n");
    } while (*addr);
    shmctl(shmid, IPC_RMID, 0); //撤消共享存储区，归还资源
    exit(0);
}

int main()
{
    int p;
    p = fork(); //创建子进程
    if(p < 0){
        printf("子进程创建失败！\n");
    }
    else if(p == 0){
        server();
    }else{
        client();
    }

}

