#include "../include/write.h"

int writeStr(int fifo, const char *buf)
{
    ssize_t bytes_written = write(fifo, buf, sizeof(buf));
    if(bytes_written == -1)
    {
        perror("Error: writing to fifo.");
        return -1;
    }
    return 0;
}
