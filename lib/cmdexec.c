#include "shell_hdr.h"
#include <sys/wait.h>

void cmdexec(const char *buf, char*	args[MAX_ARG], struct cmd_feature* cmdfea){
    pid_t	pid;
	int		status;
    
    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {		/* child */
        printf("\e[35mcmd execute by pid %ld\e[0m\n",(long)getpid());
        if(cmdfea->isbg){
            setpgid(0,0);               //if run back, let child process to new process group
        }
        signal(SIGINT, SIG_DFL);        //set child process signal default deal
        signal(SIGTSTP, SIG_DFL);
        signal(SIGCONT, SIG_DFL);
        dup2(cmdfea->in_fd,STDIN_FILENO);   //to redir
        dup2(cmdfea->out_fd,STDOUT_FILENO);
        execvp(args[0], args);
        err_ret("couldn't execute: %s", buf);
        exit(127);
    }
    
    if(cmdfea->in_fd!=0)close(cmdfea->in_fd);  //in the parent process close redir fd
    if(cmdfea->out_fd!=1)close(cmdfea->out_fd);
    /* parent */
    if (cmdfea->isbg) 
        signal(SIGCHLD, SIG_IGN);            //if run back, let process ignore the child signal
    else {
        if ((pid = waitpid(pid, &status, WUNTRACED)) < 0&&errno!=ECHILD)
        err_sys("waitpid error");
    }
    int i=0;
    while(args[i]!=NULL)free(args[i++]);
}