#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>

#define PAGE_NUM 32
#define PAGE_SIZE 10
#define CACHE_SIZE 8 // 缓存中最多容纳8个页面
#define DELTA 80     // 工作集大小

int *generate_instruction_sequence()
{
    srand(time(NULL)); // 设置种子
    int *instruction_seq = (int *)malloc(sizeof(int) * 320);
    int iter = 0;
    int begin = 0;
    int front_addr = 0;
    while (iter < 320)
    {
        begin = rand() % 318; // 每次随机选取起点 // 减了点范围防止溢出
        instruction_seq[iter++] = begin + 1;
        front_addr = rand() % (begin + 2);
        instruction_seq[iter++] = front_addr;     // 前地址
        instruction_seq[iter++] = front_addr + 1; // 顺序
        // instruction_seq[iter++] =  319 - (rand() % (begin + 3));  // 后地址，通过集合运算计出
        instruction_seq[iter++] = begin + 2 + rand() % ((319 - (begin + 2)) + 1); // 加一防止除零错误
    }
    return instruction_seq;
}

int *convert_to_page_current(int *seq_ins)
{
    int *page_addr_seq = (int *)malloc(sizeof(int) * 320);
    for (int i = 0; i < 320; ++i)
    {
        page_addr_seq[i] = seq_ins[i] / 10;
    }
    return page_addr_seq;
}

int *convert_to_page_current_by_work_set(int *page_seq)
{

    int begin = 0;
    while (begin < 320)
    {
        int array_for_num[DELTA];
        // 未初始化导致了段错误
        for (int i = 0; i < DELTA; ++i)
        {
            array_for_num[i] = 0;
        }
        for (int i = begin; i < begin + DELTA; ++i)
        {
            array_for_num[page_seq[i]]++;
        }
        begin += DELTA;

        int pos = 0;
        while (true)
        {
            int count = 0;
            int i = rand() % DELTA;
            while (array_for_num[i] == 0)
            {
                i++;
                count++;
                if (i >= DELTA)
                {
                    i = 0;
                }
                if (count == DELTA)
                {
                    break;
                }
            }
            if (count == DELTA)
            {
                break;
            }
            assert(array_for_num[i] != 0);
            for (int j = 0; j < array_for_num[i]; ++j)
            {
                page_seq[pos++] = i;
            }
            array_for_num[i] = 0;
        }
    }
    return page_seq;
}

void opt(int *page_ins) // LFU算法模拟
{
    int hits = 0; // 命中次数
    int miss = 0; // 缺页次数
    int cache[CACHE_SIZE];
    int visit[CACHE_SIZE]; // 用visit数组的数值模拟对未来的访问趋势，较大说明最近被访问的可能性大
    // init the cache
    for (int i = 0; i < CACHE_SIZE; ++i)
    {
        cache[i] = -1; // 不可达
        visit[i] = 0;
    }
    int pos = 0;  // 记录需要替换的位置信息
    int flag = 0; // 记录是否命中
    for (int j = 0; j < 320; ++j)
    {
        for (int k = 0; k < CACHE_SIZE; ++k)
        {
            // 寻找是否已在cache中
            if (cache[k] == page_ins[j])
            { // hit
                hits++;
                flag = 1;
                visit[k]++;
                break;
            }
        }
        // 未命中
        if (flag == 0)
        {
            miss++;
            // 缺页处理
            int min = 100; // 记录
            for (int i = 0; i < CACHE_SIZE; ++i)
            {
                if (cache[i] == -1)
                {
                    // 有空位，直接入缓存
                    visit[i] = 1;
                    pos = i;
                    break;
                }
                // 找到最小的插入位置即最不可能被使用的位置
                min = min < visit[i] ? min : visit[i];
                if (min == visit[i])
                {
                    pos = i;
                }
            }
            cache[pos] = page_ins[j];
            visit[pos] = 1;
        }
        flag = 0;
    }
    printf("opt执行完成, 缺页次数：%d, 命中次数：%d, 缺页率：%f%%\n", miss, hits, (float)(miss / 3.2));
}

void fifo(int *page_ins)
{
    int hits = 0; // 命中次数
    int miss = 0; // 缺页次数
    int cache[CACHE_SIZE];
    int flag = 0;

    int now = 0; // 记录cache的位置
    int old = 0;

    // init the cache
    for (int i = 0; i < CACHE_SIZE; ++i)
    {
        cache[i] = -1; // 不可达
    }
    for (int j = 0; j < 320; ++j)
    {
        for (int k = 0; k < CACHE_SIZE; ++k)
        {
            // 是否命中
            if (cache[k] == page_ins[j])
            {
                hits++;
                flag = 1;
                // visit[k]++;
                break;
            }
        }
        // 未命中
        if (flag == 0)
        {
            miss++;
            // 对空位的处理
            for (int k = 0; k < CACHE_SIZE; ++k)
            {
                if (cache[k] == -1)
                {
                    cache[k] = page_ins[j];
                    now = k;
                    break;
                }
            }
            // 不为空则替换先进的页面
            // 第一次替换的页面肯定是0，所以用了一个新变量表示
            cache[old] = page_ins[j];
            old++;
            if (old >= 10)
            {
                old = 0; // 使old在0-9的范围内循环
            }
        }
        flag = 0; // 这里忘了那么程序就不会进行缺页处理
    }
    printf("fifo执行完成, 缺页次数：%d, 命中次数：%d, 缺页率：%f%%\n", miss, hits, (float)(miss / 3.2));
}

/*
正规的LRU算法是用哈希表加链表实现的
同时还有一些插入变更的方法，要是用c语言还是用一种简单的模拟法来完成
*/
void lru(int *page_ins)
{
    int hits = 0; // 命中次数
    int miss = 0; // 缺页次数
    int flag = 0;
    int cache[CACHE_SIZE];
    // init the cache
    for (int i = 0; i < CACHE_SIZE; ++i)
    {
        cache[i] = -1; // 不可达
    }
    for (int i = 0; i < 320; ++i)
    {
        for (int j = 0; j < CACHE_SIZE; ++j)
        { // 命中
            if (cache[j] == page_ins[i])
            {
                // 将新访问的元素放到队首
                flag = 1;
                hits++;
                for (int k = j; k > 0; --k)
                {
                    cache[k] = cache[k - 1];
                }
                cache[0] = page_ins[i];
                break;
            }
        }
        if (flag == 0)
        {
            // 未命中 将尾部值移除 插入新值
            miss++;
            for (int j = 9; j > 0; --j)
            {
                cache[j] = cache[j - 1];
            }
            cache[0] = page_ins[i];
        }
        flag = 0;
    }

    printf("lru执行完成, 缺页次数：%d, 命中次数：%d, 缺页率：%f%%\n", miss, hits, (float)(miss / 3.2));
}

int main()
{
    int *ins_seq = generate_instruction_sequence();
    /*
    for (int i = 0; i < 320; ++i) {
        printf("%d\n", ins_seq[i]);
    }*/
    int *page_ins = convert_to_page_current(ins_seq);

    opt(page_ins);
    fifo(page_ins);
    lru(page_ins); // TODO()

    // work set
    printf("\n剩下是工作集的算法测试:\n");
    // printf("(工作集对缺页率的影响还与工作集大小delta和页数大小有关.)\n\n");
    int *work_set_seq = convert_to_page_current_by_work_set(page_ins);
    /*for (int i = 0; i < 320; ++i) {
        printf("%d\n", work_set_seq[i]);
    }*/

    opt(work_set_seq);
    fifo(work_set_seq);
    lru(work_set_seq);

    free(ins_seq);
}