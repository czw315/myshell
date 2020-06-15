#include "shell_hdr.h"
#include <sys/wait.h>


int
main(void)
{
	char	buf[MAXLINE];	/* from shell_hdr.h */
	char*	args[MAX_ARG];  /* to use store args */
	int     isIncmd;

	//to stop signal to parent process

	signal(SIGINT, SIG_IGN); 
    signal(SIGTSTP, SIG_IGN);
	signal(SIGSTOP,SIG_IGN);
    signal(SIGCONT, SIG_DFL);

	setShelltip(); /* print prompt */
	while (getcmdline(buf)!=-1) {		//get cmd line
		struct  cmd_feature cmdfea={.isbg=0,.out_fd=1,.in_fd=0};
		if(parsecmdline(buf,args,&cmdfea)!=-1){
			isIncmd=builtin_cmd(args);
			if(isIncmd==-1){
				cmdexec(buf,args,&cmdfea);
			}
		}
		setShelltip();
	}
	exit(0);
}
