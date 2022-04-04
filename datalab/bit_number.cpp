// 笔记只记录一些个人认为比较有价值的思路

// 不用if else等语句， !x 可以将非0的数打印0, 而!0 为 1,而!!即可将非0返回1
// 下方代码测试成功
int32_t judgePostiveOrNegative(int32_t x){
    // 判断正负，正数返回1，负数返回0
    int a = -1<<31;
    int Po = x & a;
    return !Po;
}


int negateBits(int x){
    return ~x +1;
}


int32_t isAsciiDigit(int32_t x){
    int sign = 1 << 31; // 用于判断符号位
    int upperBound = ~(sign + 0x39); // 或者用 ~(sign | 0x39)
    int lowerBound = ~0x30 + 1;
    upperBound = sign&(upperBound+x) >> 31; // 需要为0，即相加结果为正数
    lowerBound = sign&(lowerBound +x) >> 31; // 需要为0，即相加结果为正数
    return !(upperBound|lowerBound);
}

// 某种意义上实现了if else操作, 我觉得非常巧妙, 利用了 全0 和 全 1 与另一个数的 & 操作.
/*
 * conditional - same as x ? y : z
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int conditional(int x, int y, int z) {
    x = !!x;
    x = ~x+1;
    return (x&y)|(~x&z);
}

// 打印32位int变量
void printBits(int32_t x){
    //    bitset可以打印二进制数
    bitset<32> bits = bitset<32> (x);
    cout << bits << endl;
}
