
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

#define SERVER_FIFO "./SERVER_FIFO"
#define CLIENT_FIFO "./CLI_%d_FIFO"
#define READ_SIZE 44  /*Depends on the clients structure size*/
#define _PP( msg, ...) printf("%3d:%s::"#msg"\n", __LINE__,__FILE__, ##__VA_ARGS__)
#define SAFE(p) (p ? p : ({printf("\n-----Accessing NULL POINTER\n");p;}))
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
	_PP(structsize=%d\tpid=%d\ndata=%s\tstrlength=%d,
		SAFE(m)->dtsize, SAFE(m)->pid, SAFE(m)->data, SAFE(m)->strlength);
}
uchar open_fifo (char *fifo, uchar mode){
	uchar fd;
	_PP(Opening %s\n,fifo);
	fd = open(fifo, mode);
	if(fd == -1) error_call("Open Fail");
	else _PP(Sucessfully opened %s, fifo);
	return fd;
}
void read_fifo(uchar fd, char *buf, uchar size){
	uchar count;
	count =	read(fd, buf, size);
	if(count == -1) error_call("Server Read Fail!");
	else _PP(READ %d bytes from SERVERFIFO,count);
}
void write_fifo(uchar fd, char *buf, uchar size){
	uchar count;
	_PP(msg:: %s :: fd=%d, buf, fd);
	count =	write(fd, buf, size);
	if(count == -1) error_call("Server Write Fail!");
	else _PP(Wrote %d bytes into client fifo, count);
}
int main()
{
	uchar fd,count;
	uchar i;
	mydata *md = '\0';
	char rbuf[50];
	char cli_fifo[20];

	if(access(SERVER_FIFO, F_OK) == -1){
		if(mkfifo(SERVER_FIFO, 0777)) error_call("SERVERFIFO creation failed");
		else _PP(SERVERFIFO Creation Sucessfull);
	}
	for(i=0;i<3;i++){	
		fd = open_fifo(SERVER_FIFO, O_RDONLY);
		read_fifo(fd, rbuf, READ_SIZE);
		md = (mydata *)rbuf;
		printfmsg(SAFE(md));
		sprintf(cli_fifo, CLIENT_FIFO, SAFE(md)->pid);
		_PP(Go to sleep for 1 secs\n);
		sleep(1);
		fd = open_fifo(cli_fifo, O_WRONLY);
		write_fifo(fd, SAFE(md)->data, SAFE(md)->strlength);
	}
	_PP(End\n);
	
	return 0;
}
