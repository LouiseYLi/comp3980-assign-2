#include "../include/read.h"

int readStr(int fifo, size_t size)
{
    char   *buf        = (char *)malloc(size);
    ssize_t bytes_read = read(fifo, buf, size);
    if(bytes_read == -1)
    {
        perror("Error: reading from fifo.");
        free(buf);
        return -2;
    }
    display(buf);
    free(buf);
    return 0;
}
