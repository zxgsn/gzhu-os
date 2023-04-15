#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>

//
bool judge(int **have, int **max, int *total_on_os, int process_num, int resource_num)
{
    // step1 => caculate the need array
    // 在need矩阵每行的第0位添加上标记位， 如果为0说明已经执行过
    int init_process_num = process_num;
    int **need = (int **)malloc((process_num) * sizeof(int *));
    for (int i = 0; i < process_num; ++i)
    {
        need[i] = (int *)malloc((resource_num + 1) * sizeof(int)); // 注意多分配了一位
    }
    // init
    for (int i = 0; i < process_num; ++i)
    {
        need[i][0] = i + 1; // 初始位设置成进程号
        for (int j = 1; j < resource_num + 1; ++j)
        {
            need[i][j] = max[i][j - 1] - have[i][j - 1];
        }
    }
    while (process_num > 0)
    {
        int in_num = process_num;
        for (int i = 0; i < init_process_num; ++i)
        {
            if (need[i][0] == 0)
            { // 成功执行
                continue;
            }
            else
            {
                // 判断是否能执行进程
                for (int j = 1; j < resource_num + 1; ++j)
                {
                    if (need[i][j] > total_on_os[j - 1])
                    {
                        break; // 不能执行
                    }
                    if (j == resource_num)
                    { // 执行到这说明资源满足，能执行
                        need[i][0] = 0;
                        printf("%d进程成功执行\n", i);
                        process_num--;
                        // 更新total
                        for (int k = 0; k < resource_num; k++)
                        {
                            total_on_os[k] += need[i][k + 1];
                        }
                    }
                }
            }
        }
        if (in_num == process_num)
        { // 说明没有进程被执行
            printf("执行失败, 无法分配资源\n");
            for (int i = 0; i < init_process_num; ++i)
            {
                free(need[i]);
            }
            free(need);
            return false;
        }
    }
    for (int i = 0; i < init_process_num; ++i)
    {
        free(need[i]);
    }
    free(need);
    assert(process_num == 0);
    printf("成功执行完毕！\n");
    return true;
}

/*
a example for banker
       have       max         need(max - have)      total_on_os
1      0 0 1 2    0 0 1 2     caculate by judge()   2 1 0 0
2      2 0 0 0    2 7 5 0
3      0 0 3 4    6 6 5 6
4      2 3 5 4    4 3 5 6
5      0 3 3 2    0 6 5 2
*/

int main()
{
    // init
    srand(time(NULL)); // 设置种子
    int process_num = rand() % 5 + 5;
    int resource_num = rand() % 5 + 1;
    int **have = (int **)malloc(sizeof(int *) * process_num);
    int **max = (int **)malloc(sizeof(int *) * process_num);
    for (int i = 0; i < process_num; ++i)
    {
        have[i] = (int *)malloc((resource_num) * sizeof(int));
    }
    for (int i = 0; i < process_num; ++i)
    {
        max[i] = (int *)malloc((resource_num) * sizeof(int));
    }
    int *total_os_res = (int *)malloc(resource_num * sizeof(int));

    // 模拟随机动态分配
    for (int i = 0; i < process_num; ++i)
    {
        for (int j = 0; j < resource_num; ++j)
        { // 输入
            have[i][j] = rand() % 3;
            max[i][j] = rand() % 5;
            total_os_res[j] = rand() % 3 + 2;
        }
    }
    // printf
    printf("process_num: %d\n", process_num);
    printf("resores_num: %d\n", resource_num);
    for (int i = 0; i < process_num; ++i)
    {   
        printf("进程%d: ", i);
        for (int j = 0; j < resource_num; ++j)
        {
            printf("have:%d max: %d  ", have[i][j], max[i][j]);
        }
        printf("\n");
    }

    printf("系统资源：");
    for (int i = 0; i < resource_num; ++i) {
        printf("资源 %d : %d  ", i, total_os_res[i]);
    }
    printf("\n");
    judge(have, max, total_os_res, process_num, resource_num);

    // free
    for (int i = 0; i < process_num; ++i)
    {
        free(have[i]);
    }
    free(have);
    for (int i = 0; i < process_num; ++i)
    {
        free(max[i]);
    }
    free(max);
    free(total_os_res);
}