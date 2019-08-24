#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

#define SERVER_FIFO "./SERVER_FIFO"
#define CLIENT_FIFO "./CLI_%d_FIFO"
#define _PP( msg, ...) printf("%3d:%s::"#msg"\n", __LINE__,__FILE__, ##__VA_ARGS__)
typedef unsigned char uchar;
typedef struct Data{
	uchar dtsize;
        pid_t pid;
	struct{
		uchar strlength;
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
uchar open_fifo (char *fifo, uchar mode){
	uchar fd;
	_PP(Opening %s\n,fifo);
	fd = open (fifo, mode);
	if(fd == -1) error_call("Client<3> Open Fail");
	else _PP(Sucessfully opened %s, fifo);
	return fd;
}
void read_fifo(uchar fd, char *buf, uchar size){
	uchar count;
	count =	read(fd, buf, size);
	if(count == -1) error_call("Client<3> Read Fail!");
	else _PP(READ %d bytes from Client<3>FIFO,count);
	buf[count] = '\0';
}
void write_fifo(uchar fd, mydata *buf, uchar size){
	uchar count;
	count =	write(fd, buf, size);
	if(count == -1) error_call("Client<3> Write Fail!");
	else _PP(Wrote %d bytes into server fifo, count);
}

int main()
{
	uchar fd,count;
	mydata md;
	char cli_fifo[20];
	char rbuf[50];
	/* Initializing values*/		
	_PP(START\n---------);
	md.dtsize = sizeof(mydata);
	md.pid = getpid();
	strcpy(md.data,"MSG DATA::Client<3> MESSAGE");
	md.strlength = strlen(md.data);
	/*Operation*/
	printfmsg(&md);
	fd = open_fifo(SERVER_FIFO, O_WRONLY);
	write_fifo(fd, &md, md.dtsize);

	sprintf(cli_fifo, CLIENT_FIFO, md.pid);
	if(access(cli_fifo, F_OK) == -1)
        {
                if(mkfifo(cli_fifo, 0777)) error_call("Client<3>FIFO creation failed");
                else _PP(Client<3>FIFO Creation Sucessfull);
        }
	fd = open_fifo(cli_fifo, O_RDONLY);
	read_fifo(fd, rbuf, md.strlength);
	_PP(Output: %s, rbuf);
	_PP(End\n);
	
	return 0;
}
