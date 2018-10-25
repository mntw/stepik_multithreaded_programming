#include <stdio.h> // printf
#include <sys/types.h>  // for socket
#include <sys/socket.h> // for socket
#include <netinet/in.h> 
#include <getopt.h> // for options

#ifndef MSG_NOSIGNAL
#define MSG_NOSIGNAL SO_NOSIGPIPE
#endif

int is_server = 0, is_client= 0, opindex = 0, opchar = 0;

struct option opts[] = {
   {"server", no_argument, 0, 's'},
   {"client", no_argument, 0, 'c'},
   {0,0,0,0},
};

struct sockaddr_in crSockAddr (int port, uint32_t addr) {
    struct sockaddr_in SockAddr;
    SockAddr.sin_family = AF_INET; // IPv4
    SockAddr.sin_port = htons(port); // port number to network series of bytes
    SockAddr.sin_addr.s_addr = htonl(addr); // 0.0.0.0
    return SockAddr;
}

int server () {
    int MasterSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    struct sockaddr_in SockAddr =crSockAddr(12345, INADDR_ANY);
    bind(MasterSocket, 
            (struct sockaddr *) (&SockAddr), 
            sizeof(SockAddr));
    listen(MasterSocket, SOMAXCONN);
    while (1) {
        int SlaveSocket = accept(MasterSocket, 0, 0);
        int Buffer[5] ={0,0,0,0,0};
        recv(SlaveSocket, Buffer, 4, 0);
        send(SlaveSocket, Buffer, 4, 0);
        printf("%s\n", Buffer);
        close(SlaveSocket);
    }
    return 0;
}

int client() {
    int Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    struct sockaddr_in SockAddr=crSockAddr(12345, INADDR_LOOPBACK);
    connect(Socket, 
            (struct sockaddr *) (&SockAddr),
            sizeof(SockAddr));
    char Buffer [] = "ping";
    printf("%s\n", Buffer);
    send(Socket, Buffer, 4, MSG_NOSIGNAL);
    recv(Socket, Buffer, 4, MSG_NOSIGNAL);
    close(Socket);
    printf("%s\n", Buffer);
    return 0;
}

int main(int argc, char ** argv) {

    while (-1 != (opchar = getopt_long(argc, argv, "sc", opts, &opindex))) {
        switch (opchar) {
            case 'c':
               is_client = 1; 
               break;
            case 's':
               is_server = 1; 
               break;
        }
    }
    if (!(is_server ^ is_client)) {
        printf("usage: cmd -s|-c\n");
        return -1;
    }
    if (is_server) {
                printf("This is server!\n");
                return server();
    } else if (is_client) {
                printf("This is client!\n");
                return client();
    }
}
