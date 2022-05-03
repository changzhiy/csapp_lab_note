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

## 1.3 转置矩阵

### situation 1

```
// 第一次用8x8扫描的成绩

Part B: Testing transpose function
Running ./test-trans -M 32 -N 32
Running ./test-trans -M 64 -N 64
Running ./test-trans -M 61 -N 67

Cache Lab summary:
                        Points   Max pts      Misses
Csim correctness          27.0        27
Trans perf 32x32           6.9         8         343
Trans perf 64x64           0.0         8        4723
Trans perf 61x67           0.0        10     invalid
          Total points    33.9        53
                            
```

实验做了一段时间才发现, A和B会存在cache冲突，毕竟b和s总共只有10位，如果在草稿纸上演算一下就知道A，B对角线上的块会导致cache conflict.那么我们直接用8个局部变量存储A的行元素不就行了？通过这种方式,我们得到情况一的miss为287 已经是满分水准了. 理论上最少256次，即每8个元素miss一次.情况二的理论最低应为1024次，情形三由于不是8的倍数，不过差不多也是1024的最低miss情况。

> 什么时候miss最少？就是一次放入cache在使用完之前不会拿出来，那就一定miss最少。

我们尝试去接近理论最小值，还有一种好方法，就是先把元素放入B，然后在B中进行重排，但一定不能在中间造成冲突，否则这样做就没有意义了。具体来做就是把A的一行取出放入B的一行，这样取8行放8行不会造成A，B冲突，且cache中会存放B的8x8个元素，然后在B内重新对小块进行转置即可。

惊人的发现成绩达到了miss 259，已经可以进行下一个情形了。

### situation 2

这应该是最难的一个部分了.
最基础情况的miss是4000+, 如果把情形1的代码直接拷贝到2, miss就是3203.情形2由于4行就会更新cache, 所以我们采取的策略是仍用8x8扫描, 上4x8与下4x8分别处理.做法给我一种拧魔方的感觉.

[https://zhuanlan.zhihu.com/p/138881600](https://zhuanlan.zhihu.com/p/138881600)

我确实要说这种转换太精彩了. 我是错了很多次, 再参考, 再错才真体会到了这个方法的精彩. 本方法的miss惊人的达到了1100, 很接近理论最小.

### situation 3

如果用原始策略, miss 4000+.

```
// 使用8x8的策略, miss 2100+.我们没必要用提取a0~a7的策略，因为situation 3没有那么高要求并且因为它不规则, 会需要很多判断条件, 且并不一定提取出来就会降低miss
    
for (i = 0; i < N; i+=len) {
    for (j = 0; j < M; j+=len) {
        for (k = i; k < i+len&&k<N; ++k) {
            for(s=j; s < j + len && s < M;++s){
                B[s][k] = A[k][s];
            }
        }
    }
}

// 使用10x10的策略 2000+ 

// 结合situation1的策略 2000+
        // 先截取一个大正方形
        for (i = 0; i < 56; i += len) {
            for (j = 0; j < 56; j += len) {
                for (k = i, s = j; k < i + len; k++, s++) {
                    a0 = A[k][j];
                    a1 = A[k][j + 1];
                    a2 = A[k][j + 2];
                    a3 = A[k][j + 3];
                    a4 = A[k][j + 4];
                    a5 = A[k][j + 5];
                    a6 = A[k][j + 6];
                    a7 = A[k][j + 7];
                    B[s][i] = a0;
                    B[s][i + 1] = a1;
                    B[s][i + 2] = a2;
                    B[s][i + 3] = a3;
                    B[s][i + 4] = a4;
                    B[s][i + 5] = a5;
                    B[s][i + 6] = a6;
                    B[s][i + 7] = a7;
                }
                for (k = 0; k < len; k++) {
                    for (s = k + 1; s < len; s++) {
                        a0 = B[k + j][s + i];
                        B[k + j][s + i] = B[s + j][k + i];
                        B[s + j][k + i] = a0;
                    }
                }
            }
        }
        // 剩下5x67 + 61x11 (有重叠)
        for (i = 0; i < N; i+=8) {
            for (j = 56; j < M; j+=8) {
                for (k = i; k < i+8&&k<N; ++k) {
                    for(s=j; s < j + 12 && s < M;++s){
                        B[s][k] = A[k][s];
                    }
                }
            }
        }
        // 剩下5x56
        for (i = 56; i < N; i+=8) {
            for (j = 0; j < 56; j+=8) {
                for (k = i; k < i+8&&k<N; ++k) {
                    for(s=j; s < j + 8 && s < M;++s){
                        B[s][k] = A[k][s];
                    }
                }
            }
        }


    }

// 使用20x20的策略 2002, 快到满分的水准了
// 也可试试非正方形区域, 发现也没什么突破

// 参照一个博主的用16x16, 结果就如下面的1992(个人觉得如果要让situation 3再低需要自己罗列出不同坐标调用的cache, 需要非常精细的安排)
```


```
Cache Lab summary:
                        Points   Max pts      Misses
Csim correctness          27.0        27
Trans perf 32x32           8.0         8         259
Trans perf 64x64           8.0         8        1107
Trans perf 61x67          10.0        10        1992
          Total points    53.0        53
```
