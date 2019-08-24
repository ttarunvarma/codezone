#! /bin/sh

: '  For multiline Comments
1st Unlink
2nd Create 2 ELF
3rd Execute them(One in the backgroung) 

'
#find . -type l -exec unlink {} \; # Not working
ls
rm -vf server_fifo client_fifo_1 client_fifo_2 client_fifo_3

echo Unlink 2 fifo
make server_fifo client_fifo_1 client_fifo_2 client_fifo_3

echo created 2 ELF
echo Run server_fifo.exe
echo ---------------
./server_fifo &   # Executes in backgroung, which makes command prompt available

./client_fifo_1 & 
./client_fifo_2 &
./client_fifo_3

echo END SCRIPT


