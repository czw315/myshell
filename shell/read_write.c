#include<unistd.h>
#include<stdlib.h>

int main(){
    char buf[1024];
    int numread=0;
    while((numread=read(STDIN_FILENO,buf,1024))>0){
        write(STDOUT_FILENO,buf,numread);
    }
    exit(0);
}