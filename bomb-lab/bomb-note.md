必须要会的gdb操作

gdb program // to invoke gdb 

disas function //disassemble 

run xxx.txt // 传参数

/********
b xx // breakpoints,可以对代码某一行, 也可以对内存的某一行
b 82 
b *0x00007ffff7edf002
/******

p //print, 打印信息，但不能打印内存信息

/*************
layout 非常有意思, 因为一直滑轮翻来翻去很麻烦 

layout：用于分割窗口，可以一边查看代码，一边测试。主要有以下几种用法： 
layout src：显示源代码窗口 
layout asm：显示汇编窗口 
layout regs：显示源代码/汇编和寄存器窗口 
layout split：显示源代码和汇编窗口 

/*************/
