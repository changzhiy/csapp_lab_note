# Arch-Lab

## Part A

### sum.ys

如果有跟我一样一开始无从下手的可以不用着急，不用去网上找答案，这说明汇编知识很欠缺，可以去简单学一些汇编语言然后再来写y86代码，会发现简单了不少. y86-64 栈内一行8字节，自己写汇编要明确知道代码块在哪里，rsp、rip分别是什么。最后观察这个链表的值可以得出相加和为cba即正确，答案放在%rax中.

```
#Execution begins at address 0
# y86-64 栈内一行8字节，自己写汇编要明确知道代码块在哪里，rsp、rip分别是什么
# 返回值在rax内
	.pos 0
	irmovq stack,%rsp
	call main
	halt

	.align 8
ele1:
	.quad 0x00a
	.quad ele2
ele2:
	.quad 0x0b0
	.quad ele3
ele3:
	.quad 0xc00
	.quad 0

main:
	irmovq ele1, %rdi #rdi 指向element的地址
	xorq %rax,%rax #sum = 0
    call sum
	ret


sum:
	jmp test
  

loop:
	mrmovq (%rdi),%r10 
	addq %r10,%rax # sum + val
	mrmovq 8(%rdi), %rdi # 这里要注意一下，这里的地址都是指向val或者下一个链表的地址，都需要取数操作

	

test:
	andq %rdi,%rdi
	jne loop #stop when 0
	ret 

	.pos 0x200

stack:
```

### rsum.ys

这里稍微注意的点就是栈的使用, rsum观察c语言代码的话就是一个递归的过程，我们如果要模拟这个过程的话就需要用到栈存值

```
.pos 0
	irmovq stack,%rsp
	call main
	halt

	.align 8
ele1:
	.quad 0x00a
	.quad ele2
ele2:
	.quad 0x0b0
	.quad ele3
ele3:
	.quad 0xc00
	.quad 0

main: 
	irmovq ele1,%rdi
	xorq %rax,%rax
	call sum
	ret

sum:
	mrmovq (%rdi),%r10
	pushq %r10
	mrmovq 8(%rdi),%rdi
	call r_sum # 因为指令的限制，只能调用函数来模拟递归
	popq %r10
	addq %r10,%rax

r_sum:
	andq %rdi,%rdi
	jne sum
	ret 



	.pos 0x200

stack:
```

## partB

和汇编语言一样，不了解一些的话都不知道实验的目的是什么，简要阐述一下就是硬件控制语言用注释表明了不同阶段，如果你认为该指令需要哪些阶段，你就把指令名加入哪些阶段即可，并不是真让你写硬件控制语言。

如果懂了以后就比较容易了，和课本上的案例一模一样的。不过测试的时候还有一点小问题，如果提示没有tk文件，就需要我们手动vim Makefile，看见英文注释中有该问题解决方案，我们注释掉相应行即可。接着按文档来就可以，最后一点要注意的有一步要我们自己化ys文件为yo, 这个利用partA的工具即可。

当看到模拟的结果都是success的时候就可以了。
