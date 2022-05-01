开始前, 介绍一个linux命令"man". man命令是Linux下最核心的命令之一。而man命令也并不是英文单词“man”的意思，它是单词manual的缩写，即使用手册的意思。man命令会列出一份完整的说明。 其内容包括命令语法、各选项的意义及相关命令 。更为强大的是，不仅可以查看Linux中命令的使用帮助，还可以查看软件服务配置文件、系统调用、库函数等帮助信息。

# 1. CacheLab

## 1.1 getopt

```
// 说实话我C语言的基础就是没有基础, 咱还是慢慢记慢慢学吧
// 可以用gcc生成exe文件  gcc getopt.c -o geto, 顺序错了会删除文件, 我也不知道为啥
// atoi的意思就是string to int 这个在后面会有大用处
// 另外还要学习malloc, 结构体定义等等, 没有C的基础是做不了这个实验的.

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv) {
    int opt, x, y;
    /* looping over arguments */
    while (-1 != (opt = getopt(argc, argv, "x:y:"))) { // 这个冒号不是随意填写的
        冒号代表后面需要参数, "x:y:hf:"就代表x，y，f需要，h不需要, 然后也调试成功了
        /* determine which argument it’s processing */
        switch (opt) {
            case 'x':
                x = atoi(optarg);
                break;
            case 'y':
                y = atoi(optarg);
                break;
            default:
                printf("wrong argument\n");
                break;
        }
    }
}
```

```
// 尝试使用参数

case 'f':
printf("%s",optarg);
pFILE = fopen(optarg, "r");

while (fscanf(pFILE, " %c %x,%d", &identifier, &address, &size) > 0) {
    printf("%c", identifier);
    printf("%x", address);
    printf("%d", size);
}
fclose(pFILE);
break;

(base) program_machine@CaniandeMacBook-Pro cachelab % ./geto -f trace.txt
trace.txtL101L101M201L221S181L1101L2101M121%
```

## 1.2 LRU算法

个人认为LRU算法是第一个实验的核心吧, LRU一般有两种算法, 使用链表或者时间戳, 考虑到时间戳容易实现一点, 就采用时间戳吧, 每次找到一组里使用最少的那个元素，驱逐它就对了. 其他的部分可以参照一下别人的, 因为很多都是模版, 通过这个实验也是学习了很多知识, 虽然很多不是自己写的.

```
// 这是我仿照大佬写的一个LRU示例, 原理就是最少使用的元素stamp最大, 每次把最大的evict然后就可以拿到空位. 
#include "stdlib.h"
#include "stdio.h"

typedef struct node{
    int valid;
    int stamp;
    int val;
}node,*p_n;

int SIZE = 5;

p_n init(){
    p_n p = (p_n)malloc(sizeof (node) * SIZE);
    for(int i =0 ;i < SIZE; ++i){
        p[i].valid = 0;
        p[i].stamp = -1;
        p[i].val = -1;
    }
    return p;
}

int get_max_stamp(p_n pN){
    int max_i = 0;
    int max = pN[0].stamp;
    for (int i = 1; i < SIZE; ++i) {
        if (pN[i].valid == 1 && pN[i].stamp > max || (pN[max_i].valid == 0  && pN[max_i].valid == 1)){
            max_i = i;
            max = pN[i].stamp;
        }
    }
    return max;
}

void update_LRU(p_n pN,int val){
    for (int i = 0; i < SIZE; ++i) {
        if (pN[i].valid == 0){
            pN[i].val = val;
            pN[i].stamp = 0;
            pN[i].valid = 1;
            printf("store success\n");
            return;
        }
    }

    for (int i = 0; i < SIZE; ++i) {
        if (pN[i].val == val && pN[i].valid == 1){
            pN[i].stamp = 0;
            printf("find success\n");
            return;
        }
    }


    // LRU
    for (int i = 0; i < SIZE; ++i) {
        if (pN[i].stamp == get_max_stamp(pN)) {
            pN[i].stamp = 0;
            pN[i].valid = 1;
            pN[i].val = val;
            printf("evict success\n");
            return ;
        }
    }
}

void update_stamp(p_n pN){
    for (int i = 0; i < SIZE; ++i) {
        if(pN[i].valid == 1){
            ++pN[i].stamp;
        }

    }
}


int main(){
    p_n pn = init() ;
    for(int i = 0; i < 50;++i){
        int temp = i >> 2;
        update_LRU(pn,temp);
        update_stamp(pn);
        printf("1:%d,2:%d,3:%d,4:%d,5:%d",pn[0].stamp,pn[1].stamp,pn[2].stamp,pn[3].stamp,pn[4].stamp);
    }
}


```
