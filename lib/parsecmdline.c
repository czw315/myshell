#include "shell_hdr.h"
#include<fcntl.h>

int parsecmdline(char *command, char* args[],struct cmd_feature* cmdfea) { // 以空格分割命令， 返回分割得到的字符串个数
    char buf[MAXLINE];
    if(command[strlen(command)-1]=='&'){    //solve run in the back
        cmdfea->isbg=1;
        command[strlen(command)-1]=0;
	}
    strcpy(buf,command);
    char* token;
    int num=0;
    token=strtok(buf," ");                  //use strtok to split args
    while(token!=NULL){
        args[num]=malloc(strlen(token)+1);
        strcpy(args[num++],token);
        token=strtok(NULL," ");
    }
    args[num]=NULL;         
    int inredi_times=0;
    int outredi_times=0;
    for(int i=0;i<num;i++){                 //judge has redir option
        if(strcmp(args[i],"<")==0){
            inredi_times++;
            if(inredi_times==1&&i+1<num){
                cmdfea->in_fd=open(args[++i],O_RDONLY);
                if(cmdfea->in_fd==-1){
                    err_ret("inredi open error");
                    return -1;
                }
                args[i]=NULL;               //set args about redi to NULL
                args[i-1]=NULL;
            }else{
                err_ret("inredi args error");
                return -1;
            }
        }else if(strcmp(args[i],">")==0){
            outredi_times++;
            if(outredi_times==1&&i+1<num){
                cmdfea->out_fd=open(args[++i],O_WRONLY);
                if(cmdfea->out_fd==-1){
                    err_ret("outredi open error");
                    return -1;
                }
                args[i]=NULL;
                args[i-1]=NULL;
            }else{
                err_ret("outredi args error");
                return -1;
            }
        }
    }
    for(int i=0,j=0;j<num;j++){         //move NULL args to the end
        if(args[j]){
            args[i]=args[j];
            i++;
        }
    }
    return 0;
}