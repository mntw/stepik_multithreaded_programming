#include <iostream> // std::cout
#include <fstream>  // std::ofstream
#include <unistd.h> // getpid

#define ERROR_CREATE_THREAD -11
#define ERROR_JOIN_THREAD   -12
#define SUCCESS        0

/*
Напишите многопоточную программу (POSIX Threads).
Должны быть как минимум:
Один поток, ожидающий события по условной переменной.
Один поток, ожидающий достижения барьера.
Текст программы должен находиться в файле /home/box/main.cpp
PID запущенного процесса должен быть сохранен в файле /home/box/main.pid
 */


pthread_cond_t cv = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mx = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mx2 = PTHREAD_MUTEX_INITIALIZER;
pthread_barrier_t bar;
//pthread_once_t once_ctrl = PTHREAD_ONCE_INIT;
//pthread_once(&once_ctrl, void (* init_routine (void)));
const unsigned int n_thread = 5;


void * wait_for_cv (void *args) {
    std::cout << "Waiting for signal\n";
    pthread_cond_wait(&cv, &mx);
    //pthread_cond_timedwait(&cv, &mx, time);
    std::cout << "CV has been released\n";
    return SUCCESS;
}
void * wait_for_barrier (void * time) {
    size_t t = * (size_t *)  time;
    pthread_mutex_unlock(&mx2);
    printf("I am new thread and i will sleep for %lu  seconds\n", t);
    sleep(t);
    printf("I've slept for %lu  seconds. Now I am on barrier, waiting for others\n", t);
    pthread_barrier_wait(&bar);
    std::cout << "Let's move on\n";
    return SUCCESS;
}

int main(int argc, char ** argv) {
    pthread_t threads[n_thread + 1];
    pthread_cond_init(&cv, NULL);
    pthread_mutex_lock(&mx);
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    int status;

    status = pthread_create(&threads[0], &attr, wait_for_cv,NULL);
    if (status != 0) {
        printf("main error: can't create thread, status = %d\n", status);
        exit(ERROR_CREATE_THREAD);
    }

    pthread_barrier_init(&bar, NULL, n_thread);
    for (size_t i = 1; i != n_thread + 1; ++i) {
        pthread_mutex_lock(&mx2);
        size_t time = i;
        status = pthread_create(&threads[i], &attr, wait_for_barrier, &time);
        if (status != 0) {
            printf("main error: can't create thread, status = %d\n", status);
            exit(ERROR_CREATE_THREAD);
        }
    }

    sleep(8);

    pthread_cond_signal(&cv); //notify one
    //pthread_cond_broadcast(&cv); // notify all


    pthread_mutex_unlock(&mx);
    pthread_cond_destroy(&cv);
    pthread_mutex_destroy(&mx);
    pthread_mutex_destroy(&mx2);
    pthread_barrier_destroy(&bar);

    return 0;
}



