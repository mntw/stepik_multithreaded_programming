#include <iostream>
#include <string>
#include <cstdio>
#include <unistd.h>
/*
Задача на безымянные каналы.
Пусть у нас есть длинная команда вида:
who | sort | uniq -c | sort -nk1
Надо ее прочитать из STDIN, выполнить и STDOUT записать в файл /home/box/result.out
Вы должны создать Makefile в домашнем каталоге.
Тестовая система выполнит make. Она ожидает, что появится файл исполняемый файл - /home/box/shell. После чего она исполнит его несколько раз, подавая на STDIN разные строчки и проверяя result.out.
Пожалуйста, ВЕЗДЕ используйте абсолютные пути!
 */
using namespace std;


void who (int pfd[2]) {
    close(STDOUT_FILENO);
    dup2(pfd[1], STDOUT_FILENO);
    close(pfd[1]);
    close(pfd[0]);
    execlp("who", "who", NULL);
}


void sort (int pfd1[2], int pfd2[2]) {
    close(STDIN_FILENO);
    dup2(pfd1[0], STDIN_FILENO);
    close(pfd1[1]);
    close(pfd1[0]);
    close(STDOUT_FILENO);
    dup2(pfd2[1], STDOUT_FILENO);
    close(pfd2[1]);
    close(pfd2[0]);
    execlp("sort", "sort", NULL);
}

void uniq (int pfd1[2], int pfd2[2]) {
    close(STDIN_FILENO);
    dup2(pfd1[0], STDIN_FILENO);
    close(pfd1[0]); close(pfd1[1]);
    close(STDOUT_FILENO);
    dup2(pfd2[1], STDOUT_FILENO);
    close(pfd2[1]); close(pfd2[0]);
    execlp("uniq", "uniq", NULL);
}

void sort_nk1 (int pfd[2]) {
    close(STDIN_FILENO);
    dup2(pfd[0], STDIN_FILENO);
    close(pfd[0]);
    close(pfd[1]);
    execlp("sort", "sort", NULL);
}
void sort_p (int pfd2[2], int pfd_main[2]) {
    int pfd3[2]; pipe(pfd3);
    if (!fork()) {
        uniq(pfd3, pfd_main);
    } else {
        sort(pfd2, pfd3);
    }
}
void who_p (int pfd_main[2]) {
    int pfd2[2]; pipe(pfd2);
    if (!fork()) {
        sort_p(pfd2, pfd_main);
    } else {
        who(pfd2);
    }
}

//const string pipecmd  = "who | sort | uniq -c | sort -nk1";
int main (int argc, char ** argv) {
    int pfd[2]; pipe(pfd);
    if(!fork()) {
        who_p(pfd);
    } else {
        sort_nk1(pfd);
    }

    return 0;
}