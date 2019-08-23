#! /bin/sh

: '  For multiline Comments
1st Unlink 2 Fifo
2nd Create 2 ELF
3rd Execute them(One in the backgroung)

'
unlink server_fifo
unlink cli_*

echo Unlink 2 fifo
make server_fifo client_fifo_1

echo created 2 ELF
ls
./server &   # Executes in backgroung, which makes command prompt available
ls
./client1
ls
