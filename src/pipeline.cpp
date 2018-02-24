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


void runPipe (int pipeIn[2], int pipeOut[2], char ** args) {
    if (pipeIn[0] != STDIN_FILENO) {
        close(STDIN_FILENO);
        dup2(pipeIn[0], STDIN_FILENO);
        close(pipeIn[1]);
        close(pipeIn[0]);
    }
    if (pipeOut[1] != STDOUT_FILENO) {
        close(STDOUT_FILENO);
        dup2(pipeOut[1], STDOUT_FILENO);
        close(pipeOut[1]);
        close(pipeOut[0]);
    }
    execlp(args[0], args[1], NULL);
}


int pfdStd[2] = {STDIN_FILENO, STDOUT_FILENO};
char * Who[] = {"tree", "tree"};
char * Sort[] = {"hexdump", "hexdump"};
char * Uniq[] = {"sort", "sort"};
char * WC[] = {"wc", "wc"};
//const string pipecmd  = "who | sort | uniq -c | sort -nk1";

int main (int argc, char ** argv) {
    int pfd[2]; pipe(pfd); pid_t pID = fork();
    if(pID == 0) {
        int pfd2[2]; pipe(pfd2); pid_t pID = fork();
        if (pID == 0) {
            int pfd3[2]; pipe(pfd3); pid_t pID = fork();
            if (pID == 0) {
                runPipe(pfdStd, pfd3, Who);    //1st
            } else {
                wait(pID); runPipe(pfd3, pfd2, Sort); //2nd
            }
        } else {
            wait(pID); runPipe(pfd2, pfd, Uniq); //3rd
        }
    } else {
        wait(pID); runPipe(pfd, pfdStd, WC); //4th
    }

    return 0;
}