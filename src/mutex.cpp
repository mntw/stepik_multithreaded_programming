#include <iostream> // std::cout
#include <fstream>  // std::ofstream
#include <unistd.h> // getpid

#define ERROR_CREATE_THREAD -11
#define ERROR_JOIN_THREAD   -12
#define SUCCESS        0
#define PATH_TO_PID "/home/user/main.pid"
/*
Напишите многопоточную программу (POSIX Threads).
Должны быть как минимум:
Один поток, ожидающий освобождения мьютекса.
Один поток, ожидающий освобождения спин-блокировки.
Два потока, ожидающих освобождения RW-блокировки (один должен ожидать чтения, другой - записи).
Текст программы должен находиться в файле /home/box/main.cpp
PID запущенного процесса должен быть сохранен в файле /home/box/main.pid
 */

pthread_mutex_t mx = PTHREAD_MUTEX_INITIALIZER;
pthread_spinlock_t spin;
pthread_rwlock_t rlock = PTHREAD_RWLOCK_INITIALIZER;
pthread_rwlock_t wlock = PTHREAD_RWLOCK_INITIALIZER;

void * wait_for_mx (void *args) {
    std::cout << "Mutex thread has been created\n";
    pthread_mutex_lock(&mx);
    std::cout << "Mutex unlocked\n";
    pthread_mutex_unlock(&mx);
    pthread_mutex_destroy(&mx);
    return SUCCESS;
}

void * wait_for_spinlock (void *args) {
    std::cout << "Spinlock thread has been created\n";
    pthread_spin_lock(&spin);
    std::cout << "Spinlock unlocked\n";
    pthread_spin_unlock(&spin);
    pthread_spin_destroy(&spin);
    return SUCCESS;
}

void * wait_for_read(void *args) {
    std::cout << "Read lock thread has been created\n";
    pthread_rwlock_rdlock(&rlock);
    std::cout << "Read lock unlocked\n";
    pthread_rwlock_unlock(&rlock);
    pthread_rwlock_destroy(&rlock);
    return SUCCESS;
}
void * wait_for_write(void *args) {
    std::cout << "Write lock thread has been created\n";
    pthread_rwlock_wrlock(&wlock);
    std::cout << "Write lock unlocked\n";
    pthread_rwlock_unlock(&wlock);
    pthread_rwlock_destroy(&wlock);
    return SUCCESS;
}



int main (int argv, char ** argc) {

    std::ofstream fd (PATH_TO_PID); // Open file and write PID as mentioned in the task
    pid_t pid = getpid();
    if (fd.is_open()) {
        fd << pid << std::endl;
        fd.close();
    }

    pthread_t thread_wait_for_mx;       // Один поток, ожидающий освобождения мьютекса.
    pthread_t thread_wait_for_spinlock; // Один поток, ожидающий освобождения спин-блокировки.
    pthread_t thread_wait_for_r;        // Поток, ожидающий освобождения R-блокировки
    pthread_t thread_wait_for_w;        // Поток, ожидающий освобождения W-блокировки


    pthread_mutex_init(&mx, 0);
    pthread_spin_init(&spin, PTHREAD_PROCESS_PRIVATE);

    pthread_mutex_lock(&mx);
    pthread_spin_lock(&spin);
    pthread_rwlock_wrlock(&rlock);
    pthread_rwlock_wrlock(&wlock);


    int status = pthread_create(&thread_wait_for_mx, NULL, wait_for_mx, NULL);
    if (status != 0) {
        printf("main error: can't create thread_wait_for_m, status = %d\n", status);
        exit(ERROR_CREATE_THREAD);
    }

    status = pthread_create(&thread_wait_for_spinlock, NULL, wait_for_spinlock, NULL);
    if (status != 0) {
        printf("main error: can't create thread_wait_for_spinlock, status = %d\n", status);
        exit(ERROR_CREATE_THREAD);
    }
    status = pthread_create(&thread_wait_for_r, NULL, wait_for_read, NULL);
    if (status != 0) {
        printf("main error: can't create thread_wait_for_r, status = %d\n", status);
        exit(ERROR_CREATE_THREAD);
    }
    status = pthread_create(&thread_wait_for_w, NULL, wait_for_write, NULL);
    if (status != 0) {
        printf("main error: can't create thread_wait_for_w, status = %d\n", status);
        exit(ERROR_CREATE_THREAD);
    }

    for (int i = 0; i != 4; ++i) {
        std::cout << "All your threads are belongs to us!\n";
        sleep(1);
    }

    pthread_mutex_unlock(&mx);
    pthread_spin_unlock(&spin);
    pthread_rwlock_unlock(&rlock);
    pthread_rwlock_unlock(&wlock);


    return 0;
}