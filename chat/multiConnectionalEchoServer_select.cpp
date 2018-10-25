#include <iostream>
#include <set>
#include <algorithm>
#include <sys/types.h> //
#include <sys/socket.h>//   for sockets
#include <netinet/in.h>//
#include <unistd.h>    //
#include <fcntl.h>     //

#ifndef MSG_NOSIGNAL
#define MSG_NOSIGNAL SO_NOSIGPIPE
#endif

int set_nonblock(int fd) {
    int flags;
#if defined(O_NONBLOCK)
    if (-1 == (flags = fcntl(fd, F_GETFL, 0)))
            flags = 0;
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
#else 
    flags = 1;
    return ioctl(fd, FIOBIO, &flags);
#endif
}


int main(int argc, char **argv) {
    // Master Socket that accept connections and creates new slave sockets
    int MasterSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 
                     
    std::set<int> SlaveSockets; 

    struct sockaddr_in SockAddr;
    SockAddr.sin_family = AF_INET;
    SockAddr.sin_port = htons(12345);
    SockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(MasterSocket, (struct sockaddr *) (&SockAddr), sizeof(SockAddr));

    set_nonblock(MasterSocket);

    listen(MasterSocket, SOMAXCONN);

    while (true) {
        fd_set Set; //set of descriptors that select will monitor max(1024)
        FD_ZERO(&Set); // at firs null this set
        FD_SET(MasterSocket, &Set); // add MasterSocket to this set
        for (auto iter : SlaveSockets) {
            // add every socket from slave set to selector's set
            FD_SET(iter, &Set); 
        }
        // find max descripror number among all socket descriptors that was 
        //  added to fd_set Set
        int Max = std::max(MasterSocket, 
                *std::max_element(SlaveSockets.begin(), SlaveSockets.end()));
        select(Max + 1, &Set, NULL, NULL, NULL); // listen all sockets up to maximal
        
        for (auto iter : SlaveSockets) {
            if(FD_ISSET(iter, &Set)) { // if descriptor has something to read
                static char Buffer[1024];
                int RecvSize = recv(iter, Buffer, 1024, MSG_NOSIGNAL); // read this to buffer
                if((RecvSize == 0) && (errno != EAGAIN)) {
                    shutdown(iter, SHUT_RDWR);
                    close(iter);
                    SlaveSockets.erase(iter);
                } else if (RecvSize != 0) {
                    send(iter, Buffer, RecvSize, MSG_NOSIGNAL);
                }
            }
        }
        if(FD_ISSET(MasterSocket, &Set)) { //after all slaves checking, check master socket on new connections
            int SlaveSocket = accept(MasterSocket, 0, 0); // connect it
            set_nonblock(SlaveSocket); // set in nonblocking mode
            SlaveSockets.insert(SlaveSocket); // and put it into the SlaveSockets set
        }
    }
    return 0;
}
