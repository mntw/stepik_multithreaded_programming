#include <iostream>
#include <mqueue.h>
#include <cstdio>
// Создайте очередь сообщения POSIX с именем: /test.mq
// Тестовая среда пошлет в нее сообщение.
// Получите его и сохраните в /home/box/message.txt

#define MAX_SIZE 100
#define QUEUE_NAME "/test.mq"

int main (int argc, char ** argv) {


    char buffer[MAX_SIZE + 1];

    FILE * msgtxt = fopen("/home/box/message.txt", "w");

    struct mq_attr attr;
    /* initialize the queue attributes */
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;        // maximal number of msgs in queue
    attr.mq_msgsize = MAX_SIZE; // maximal size of 1 msg
    attr.mq_curmsgs = 0;        // current number of msgs in queue

    mqd_t mq = mq_open(QUEUE_NAME, O_CREAT | O_RDONLY, 0644, &attr);

    ssize_t bytes_read = mq_receive(mq, buffer, MAX_SIZE, NULL);
    fwrite(&buffer, (size_t) bytes_read, 1, msgtxt);

    mq_close(mq);
    mq_unlink(QUEUE_NAME);
    fclose(msgtxt);
    return 0;
}
