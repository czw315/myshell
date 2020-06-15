#include "shell_hdr.h"
#include<signal.h>
#include<sys/wait.h>

void cd_in(const char *path){
    if(chdir(path)<0)
        err_ret("chdir");
}

void exit_in(){
    printf("\e[35mgoodbye!\n");
    exit(EXIT_SUCCESS);
}

void fg_in(pid_t pid){
    setpgid(pid,pid);       //set old_child process to new process group
    int status;
    if(tcsetpgrp(1,getpgrp())==0){      //let end_pot to the new process group
        kill(pid,SIGCONT);
        waitpid(pid, &status, WUNTRACED);
    }else
        printf("fg: job not found: %d\n", pid);
}

void bg_in(pid_t pid){
    int status;
    if (kill(pid, SIGCONT) < 0)
        printf("bg: job not found: %d\n", pid);
    else 
        waitpid(pid, &status, WUNTRACED);
}

int builtin_cmd(char*	args[MAX_ARG]){
    if(strcmp(args[0],"cd")==0){
        cd_in(args[1]);
        return 1;
    }
    else if(strcmp(args[0],"exit")==0){
        exit_in();
        return 1;
    }else if(strcmp(args[0],"fg")==0){
        pid_t pid=atoi(args[1]);
        fg_in(pid);
        return 1;
    }else if(strcmp(args[0],"bg")==0){
        pid_t pid=atoi(args[1]);
        bg_in(pid);
        return 1;
    }
    return -1;
}