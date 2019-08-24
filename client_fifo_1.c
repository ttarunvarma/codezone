#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

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

void error_call(char *s)
{
        perror(s);
        exit(EXIT_FAILURE);
}
void printfmsg(const mydata *m) {
_PP(structsize=%d\tpid=%d\ndata=%s\tstrlength=%d,m->dtsize, m->pid, m->data, m->strlength);}
int main()
{
        int fd,count;
        mydata md;
        char cli_fifo[20];
        char rbuf[50];
        /* Initializing values*/
        md.dtsize = sizeof(mydata);
        md.pid = getpid();
        strcpy(md.data,"MSG DATA::Client1 MESSAGE");
        md.strlength = strlen(md.data);
        /*Operation*/
        printfmsg(&md);
        _PP(Opening SERVERFIFO.....);
        fd = open(SERVER_FIFO, O_WRONLY);
        _PP(SERVERFIFO open sucessfully & writing on FIFO);
        count = write(fd, &md, md.dtsize);
        _PP(Wrote %d bytes on fifo by client1., count);

        sprintf(cli_fifo, CLIENT_FIFO, md.pid);
        if(access(cli_fifo, F_OK) == -1)
        {
                if(mkfifo(cli_fifo, 0777)) error_call("Client1FIFO creation failed");
                else _PP(Client1FIFO Creation Sucessfull);
        }
        _PP(Opening client1fifo:%s, cli_fifo);
        fd = open(cli_fifo, O_RDONLY);
        _PP(Client1fifo Opened successfully & reading the msg..);
        count = read(fd, rbuf, md.strlength);
        rbuf[count] = '\0';
        _PP(Read %d bytes from client1fifo:%s, count, rbuf);
        _PP(End);

        return 0;
}
