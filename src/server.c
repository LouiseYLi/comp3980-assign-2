#include "../include/display.h"
#include "../include/read.h"
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define FIFO_INPUT "./fifo/input"



int main(void)
{
    int fifoIn;



    fifoIn = open(FIFO_INPUT, O_RDWR | O_CLOEXEC);
    if(fifoIn == -1)
    {
        perror("Error: unable to open input fifo in client.");
        exit(EXIT_FAILURE);
    }

    readStr(fifoIn);

    display("server ran successfully");
    close(fifoIn);
    return EXIT_SUCCESS;
}
