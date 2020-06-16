# APUE-命令解释器shell实现报告
## 需求分析
1. 程序从控制台执行，启动后显示一个命令提示符。例如 “->”。用户可以通过给特定的环境变量赋值来改变命令提示符的形式；
2. 通过某个特殊的命令或按键组合可以正常地关闭本程序；
3. 提供后台运行机制。用户提交的任务可以通过某种指示使之在后台运行，例如：-> bg job1 \<CR>将使任务 job1 在后台运行，并马上返回给用户一个新的提示符；
4. 提供输出重定向。通过指定文件名将任务的所有输出覆盖写到文件中而不是送到标准输出上；
5. 提供输入重定向。通过指定文件名使得任务从相应的文件中去获取所需的数据，而不是从标准输入上。
***
## 详细设计
### print shel tips defined by myself
> setShelltip.c实现该功能
### 命令行的读取
> getcmdline.c实现从标准输入读取命令
### 命令的解析
> parsecmdline.c实现对读取命令的解析，提取出命令行参数以及运行的特征，是否在后台运行以及重定向\
`struct cmd_feature{ int isbg; int out_fd; int in_fd; };`\
> 该结构体实现对命令特征的统计,在parsecmdline.c中，通过判断命令最后一个字符是否为&来判断是否在后台运行；然后利用strtok函数实现对参数的分割；通过判断是否含有'<','>'来判断是否有输入输出重定向，同时判断打开文件，来判断是否出错，同时将参数指针数组所对应的置为NULL,最后将参数指针数组中的NULL全部后移。
### 执行命令
> 首先是通过fork出子进程来执行shell命令，其次根据传入的cmd的feature来进行特殊处理。首先是后台运行，这里采用让子进程成为新的进程组的组长来处理；其次，对于重定向，由于子进程继承父进程的文件描述符，因此，在父进程关掉这些描述符，在子进程中利用dup2来重定向。
### 内建命令
1. cd:利用chdir完成
2. exit:实现退出shell
3. fg:将后台进程在前台运行，参数为进程号
4. bg:将前台进程移动后台运行
***
## 测试
```
czw@ubuntu:~/myshell/shell$ ./shell 
czw in /home/czw/myshell/shell
sh -> exit
goodbye!
czw@ubuntu:~/myshell/shell$ ./shell 
czw in /home/czw/myshell/shell
sh -> czw@ubuntu:~/myshell/shell$
```
> 程序启动后会输出当前用户以及所在的路径\
> 输入exit或者CTRL+D可以停止，输入exit会提示goodbye!
```
czw in /home/czw/myshell/shell
sh -> ls
cmd execute by pid 3976
a.out  input_file  Makefile  output_file  prog	read_write.c  shell  shell.c
czw in /home/czw/myshell/shell
sh -> ls -l
cmd execute by pid 3977
total 72
-rwxrwxr-x 1 czw czw  8712 Jun 15 21:39 a.out
-rw-rw-r-- 1 czw czw   110 Jun 15 20:40 input_file
-rw-rw-r-- 1 czw czw   142 Jun  6 00:53 Makefile
-rw-rw-r-- 1 czw czw    10 Jun 15 21:40 output_file
-rwxrwxr-x 1 czw czw  8656 Jun  7 01:32 prog
-rw-rw-r-- 1 czw czw   204 Jun 15 21:39 read_write.c
-rwxrwxr-x 1 czw czw 28488 Jun 15 21:35 shell
-rw-rw-r-- 1 czw czw   629 Jun 15 00:25 shell.c
czw in /home/czw/myshell/shell
sh -> 
```
> 执行的命令可以带参数
```
czw in /home/czw/myshell/shell
sh -> cat input_file
cmd execute by pid 3992
count to 0
count to 1
count to 2
count to 3
count to 4
count to 5
count to 6
count to 7
count to 8
count to 9
czw in /home/czw/myshell/shell
sh -> ./a.out < input_file
cmd execute by pid 3993
count to 0
count to 1
count to 2
count to 3
count to 4
count to 5
count to 6
count to 7
count to 8
count to 9
czw in /home/czw/myshell/shell
sh -> cat output_file
cmd execute by pid 3996
cuizhiewi
czw in /home/czw/myshell/shell
sh -> echo changed > output_file
cmd execute by pid 3997
czw in /home/czw/myshell/shell
sh -> cat output_file
cmd execute by pid 3999
changed
czw in /home/czw/myshell/shell
sh ->
```
> 支持输入输出重定向
```
czw in /home/czw/myshell/shell
sh -> ./prog > tmp.txt &
czw in /home/czw/myshell/shell
sh -> cmd execute by pid 4006
ls
cmd execute by pid 4008
a.out  input_file  Makefile  output_file  prog	read_write.c  shell  shell.c  tmp.txt
```
> 支持后台运行，立刻返回新的提示符，可以执行其他命令
```
czw in /home/czw/myshell/shell
sh -> cd ..
czw in /home/czw/myshell
sh -> 
```
> 支持内建命令cd
```
czw in /home/czw/myshell/shell
sh -> ./prog &
czw in /home/czw/myshell/shell
sh -> cmd execute by pid 4139
count to 0
lscount to 1

cmd execute by pid 4140
a.out  input_file  Makefile  output_file  prog	read_write.c  shell  shell.c  tmp.txt
czw in /home/czw/myshell/shell
sh -> count to 2
count to 3
count to 4
count to 5
fcount to 6
g count to 7
414count to 8
0
czw in /home/czw/myshell/shell
sh -> count to 9
ls
cmd execute by pid 4141
a.out  input_file  Makefile  output_file  prog	read_write.c  shell  shell.c  tmp.txt
czw in /home/czw/myshell/shell
sh -> 
```
> fg命令实现将后台命令根据进程号提取到前台运行
***
## 总结
> 完成所有的项目要求，但是细节部分仍有考虑不到之处
