#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>
#include <limits.h>
#include <unistd.h>

int SumForTwoChannel(int ch1, int ch2)
{
    // printf("在两个磁道间移动： %d, %d \n", ch1, ch2);
    if ((ch1 - ch2) > 0)
    {
        return ch1 - ch2;
    }
    else
    {
        return ch2 - ch1;
    }
}

int *GenerateDiskChannel()
{
    srand(time(NULL)); // 设置种子
    int *disk_channel = (int *)malloc(sizeof(int) * 20);
    for (int i = 0; i < 20; ++i)
    {
        disk_channel[i] = rand() % 150;
    }
    return disk_channel;
}

// 先来先服务
int FCFS(int *disk_channel)
{
    int total = 0;
    // printf("%d", disk_channel[0]);
    for (int i = 1; i < 20; ++i)
    {
        // 按顺序服务
        total += SumForTwoChannel(disk_channel[i], disk_channel[i - 1]); // 第一个无需寻道
    }
    return total;
}

// 能想到的优化是sort加双指针 -> SCAN

int cmpfunc(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}
// 扫描算法
int SCAN(int *disk_channel)
{
    int total = 0;
    int begin = disk_channel[0];
    qsort(disk_channel, 20, sizeof(int), cmpfunc);
    /*
    for (int i = 0; i < 20; ++i)
    {
        printf("%d  ", disk_channel[i]);
    }
    printf("\n");
    */
    int disk_pin = -1;
    for (int i = 0; i < 20; ++i)
    {
        if (disk_channel[i] == begin)
        {
            disk_pin = i;
            break;
        }
    }
    int j = disk_pin + 1;
    int i = disk_pin - 1;
    while (i >= 0 && j < 20)
    {
        if (abs(begin - disk_channel[i]) < abs(begin - disk_channel[j]))
        { // 左磁道更近
            total += SumForTwoChannel(begin, disk_channel[i]);
            begin = disk_channel[i];
            i--;
        }
        else
        {
            total += SumForTwoChannel(begin, disk_channel[j]);
            begin = disk_channel[j];
            j++;
        }
    }
    while (i >= 0)
    {
        total += SumForTwoChannel(begin, disk_channel[i]);
        begin = disk_channel[i];
        i--;
    }
    while (j < 20)
    {
        total += SumForTwoChannel(begin, disk_channel[j]);
        begin = disk_channel[j];
        j++;
    }

    return total;
}

// 最短寻道时间优先
int SSTF(int *disk_channel)
{
    int total = 0;
    int visited[20];
    for (int i = 0; i < 20; ++i)
    {
        visited[i] = 0;
    }
    visited[0] = 1;
    int cur = disk_channel[0]; // 默认从此开始
    for (int i = 0; i < 20; ++i)
    {
        int shortestDist = INT_MAX;
        int shortIndex = -1;
        for (int j = 0; j < 20; ++j)
        {
            if (!visited[j])
            {
                int dist = abs(disk_channel[j] - cur);
                if (dist < shortestDist)
                {
                    shortestDist = dist;
                    shortIndex = j;
                }
            }
        }
        visited[i] = 1;
        total += shortestDist;
        cur = disk_channel[i];
    }

    return total;
}

int main()
{
    for (int i = 0; i < 5; ++i)
    {   
        sleep(1);
        printf("第%d次测试\n", i + 1);
        int *disk_channel = GenerateDiskChannel();
        int total = 0;
        total = FCFS(disk_channel);
        printf("先来先服务算法走过磁道数：%d\n", total);
        total = SSTF(disk_channel);
        printf("最短寻道时间优先算法走过磁道数：%d\n", total);
        total = SCAN(disk_channel);
        printf("扫描算法算法走过磁道数：%d\n", total);
    }
}