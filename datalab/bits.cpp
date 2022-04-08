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

// 用上下界来判断元素是否位于某个区间是非常好的办法, 在很多场景都有应用, 只有我们把前人的工具甩开才能体会到更深的数字美
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


/* howManyBits - return the minimum number of bits required to represent x in
 *             two's complement
 *  Examples: howManyBits(12) = 5
 *            howManyBits(298) = 10
 *            howManyBits(-5) = 4
 *            howManyBits(0)  = 1
 *            howManyBits(-1) = 1
 *            howManyBits(0x80000000) = 32
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 90
 *  Rating: 4
 */
// 这个函数技巧比较强, 刚看到这个需求的时候觉得总不能一位一位找吧，答案显然用了一些技巧避免这么机械的操作
int howManyBits(int x) {
    int b16, b8, b4, b2, b1, b0;
    // 注意这里的sign要么为0，要么为-1, 因为负数补码右移用1填 
    int sign = x >> 31;
    
    x = (sign & ~x) | (~sign & x);//如果x为正则不变，否则按位取反（这样好找最高位为1的，原来是最高位为0的，这样也将符号位去掉了）


    b16 = !!(x >> 16) << 4; // 高16位有1，则 b16 = 16,因为至少需要16位
    x = x >> b16;
    b8 = !!(x >> 8) << 3;
    x = x >> b8;
    b4 = !!(x >> 4) << 2;
    x = x >> b4;
    b2 = !!(x >> 2) << 1;
    x = x >> b2;
    b1 = !!(x >> 1);
    x = x >> b1;
    b0 = x;
    return b16 + b8 + b4 + b2 + b1 + b0 + 1;
}
