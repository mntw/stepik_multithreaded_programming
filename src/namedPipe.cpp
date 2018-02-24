#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>


int main(int argc, char **argv) {
    const char fifo_in[] = "/home/box/in.fifo";
    const char fifo_out[] = "/home/box/out.fifo";
    mkfifo(fifo_in, 0666);
    mkfifo(fifo_out, 0666);
    int fd_in = open(fifo_in, O_RDONLY);
    int fd_out = open(fifo_out, O_WRONLY);
    size_t size ;
    char buf[512];
    while ( ( size = read(fd_in, &buf, sizeof(buf)) ) ){
        write(fd_out, &buf, size);
    }
    close(fd_in);
    close(fd_out);

    return 0;
}