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
