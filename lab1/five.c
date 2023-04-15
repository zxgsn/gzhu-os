/*
使用系统调用msgget( ),msgsnd( ),msgrev( ),及msgctl( )编制一长度为1k的消息发送和接收的程序，并分析消息的创建、发送和接收机制及控制原理。
*/

#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<unistd.h>
#include <assert.h>

struct msgbuf {
    long mtype;
    char mtext[1024]; // 题目要求长度为1k
};

static void send_msg(int msgqid, int msgtype) {
    // 分配并初始化
    struct msgbuf msg;
    snprintf(msg.mtext, sizeof(msg.mtext), "this is a message.");
    msg.mtype = msgtype;
    // 使用msgnsd函数发送消息
    if (msgsnd(msgqid, (void*) &msg, sizeof(msg.mtext), IPC_NOWAIT) == -1) {
        perror("msgsnd error");
        exit(1);
    }
    printf("\nsent: %s\n\n", msg.mtext);
}

static void get_msg(int msgqid, int msgtype) {
    struct msgbuf msg;
    // 使用msgrcv函数接收消息
    if (msgrcv(msgqid, (void*) &msg, sizeof(msg.mtext), msgtype, IPC_NOWAIT) == -1) {
        if (errno != ENOMSG) {
            perror("msgrcv");
            exit(EXIT_FAILURE);
        }
        printf("\nno message received.\n\n");
    } else {
        printf("\nmessage received: %s\n\n", msg.mtext);
    }

}

int main(int argc, char* argv[]) {
    assert(argc == 2);
    int n = atoi(argv[1]); // 0代表发送， 1代表接收
    int msgqid; // 消息队列描述符
    int msgkey = 123; // 消息队列名
    int msgtype = 1;
    // 使用msgget函数创建消息
    msgqid = msgget(msgkey, IPC_CREAT | 0400);
    // 实现在创建时检查消息队列是否已存在，如果不存在则创建一个新的消息队列 
    if (msgqid == -1) {
        perror("msgget");
        exit(1);
    }
    switch(n) {
    case 0:
        send_msg(msgqid, msgtype);
        break;
    case 1:
        get_msg(msgqid, msgtype);
        break;
    default:
        printf("error input");
    } 
}