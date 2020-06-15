#ifndef SHELL_HDR_H
#define SHELL_HDR_H

#include<sys/types.h>
#include<stdlib.h>
#include<errno.h>
#include<unistd.h>
#include<stdio.h>
#include<string.h>

#define MAXLINE 4096
#define MAX_ARG 100

//the feature of cmd : is run in the background,
//                     redirect outfile
//                     redirect infile
struct cmd_feature{
    int isbg;
    int out_fd;
    int in_fd;
};


/**
 * print shell tip : such as czw@....
 */
void setShelltip();

/**
 * get cmd line from the standard input
 */
int getcmdline(char *);
/**
 * parse the cmd line, get its feature and store to cmd_feature
 */
int parsecmdline(char *, char* [],struct cmd_feature*);

/**
 * some in_built cmd
 */
void exit_in();
void cd_in(const char *);
void fg_in(pid_t);
void bg_in(pid_t);
int builtin_cmd(char* [MAX_ARG]);


/**
 * cmd exec function
 */
void cmdexec(const char *, char* [MAX_ARG],struct cmd_feature*);


/**
 * some error function from apue
 */
void	err_msg(const char *, ...);			/* {App misc_source} */
void	err_dump(const char *, ...) __attribute__((noreturn));
void	err_quit(const char *, ...) __attribute__((noreturn));
void	err_cont(int, const char *, ...);
void	err_exit(int, const char *, ...) __attribute__((noreturn));
void	err_ret(const char *, ...);
void	err_sys(const char *, ...) __attribute__((noreturn));

#endif
