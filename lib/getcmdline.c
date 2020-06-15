#include "shell_hdr.h"

int getcmdline(char *cmd){
    if(fgets(cmd, MAXLINE, stdin)==NULL)return -1;
    if(cmd[strlen(cmd) - 1] == '\n')
        cmd[strlen(cmd) - 1] = 0; /* replace newline with null */
    return 0;
}