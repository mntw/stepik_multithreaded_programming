/*
Напишите многопоточную программу (POSIX Threads).
Должен быть как минимум:
Один поток, ожидающий pthread_join.
Текст программы должен находиться в файле /home/box/main.cpp
PID запущенного процесса должен быть сохранен в файле /home/box/main.pid
 */

#include <iostream>
#include <fstream>
#include <unistd.h>

#define ERROR_CREATE_THREAD -11
#define ERROR_JOIN_THREAD   -12
#define SUCCESS        0

void* writePID(void *args) {
    std::ofstream fd ("/home/user/main.pid");
    pid_t pid = getpid();
    if (fd.is_open()) {
        fd << pid << std::endl;
        fd.close();
    }
    return SUCCESS;
}


int main(int argc, char ** argv ) {
    pthread_t thread;
    int status = pthread_create(&thread,       // pthread_t *thread
                                NULL,          // const pthread_attr_t *attr
                                writePID,      // void *(*start_routine) (void *)
                                NULL);         // void *arg
    if (status != 0) {
        printf("main error: can't create thread, status = %d\n", status);
        exit(ERROR_CREATE_THREAD);
    }
    status = pthread_join(thread, NULL);

    if (status != SUCCESS) {
        printf("main error: can't join thread, status = %d\n", status);
        exit(ERROR_JOIN_THREAD);
    }


    return 0;
}