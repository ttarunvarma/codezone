
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

#define SERVER_FIFO "./server_fifo"
#define CLIENT_FIFO "./cli_%d_fifo"
#define _PP( msg, ...) printf("%3d:%s::"#msg"\n", __LINE__,__FILE__, ##__VA_ARGS__)

typedef struct Data{
        int dtsize;
        pid_t pid;
        struct{
                int strlength;
                char data[35];
        };
}mydata;

void error_call(char *s){
        perror(s);
        exit(EXIT_FAILURE);
}

void printfmsg(const mydata *m) {
_PP(structsize=%d\tpid=%d\ndata=%s\tstrlength=%d,m->dtsize, m->pid, m->data, m->strlength);
}

int main(){
        int fd,count;
        mydata *md = '\0';
        char rbuf[50];
        char cli_fifo[20];
        int open_mode = O_RDONLY;
        if(access(SERVER_FIFO, F_OK) == -1){
                if(mkfifo(SERVER_FIFO, 0777)) error_call("SERVERFIFO creation failed");
                else _PP(SERVERFIFO Creation Sucessfull);
        }
        _PP(Opening SERVERFIFO...);
        fd = open(SERVER_FIFO, open_mode);
        _PP(Sucessfully opened Serverfifo & reading msg);
        count = read(fd, rbuf, 50);
        _PP(READ %d bytes from SERVERFIFO,count);
        md = (mydata *)rbuf;
        printfmsg(md);
        _PP(After reading pid=%d, md->pid);
        sprintf(cli_fifo, CLIENT_FIFO, md->pid);
        _PP(Opening Client fifo:%s, cli_fifo);
        _PP(Go to sleep for 2 secs);
        sleep(2);
        fd = open(cli_fifo, O_WRONLY);
        _PP(Sucessfully opened %s & writing msg, cli_fifo);
        write(fd, md->data, md->strlength);
        _PP(Wrote %d bytes in %s, count, cli_fifo);
        _PP(End);

        return 0;
}
