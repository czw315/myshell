#include "shell_hdr.h"
#include<pwd.h>

void setShelltip(){
    char pathbuf[MAXLINE]; /* for working dir */

    /**
     * read from /etc/passwd by uid
     */
    struct passwd* pwd=getpwuid(getuid());
    if(pwd)printf("\e[34m%s\e[0m in ",pwd->pw_name);

    /**
     * get present working dir
     */
    getcwd(pathbuf, sizeof(pathbuf));
    printf("\e[34m%s\e[0m\n", pathbuf);
    printf("\e[33msh ->\e[0m ");
}