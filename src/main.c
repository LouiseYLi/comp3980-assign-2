#include "../include/display.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
    // char c = '0';
    // if(argc > 0)
    // {
    //     return EXIT_SUCCESS;
    // }
    // if(*(argv[0]) == c)
    // {
    //     return EXIT_SUCCESS;
    // }

    int fifoIn  = -1;
    int fifoOut = -1;
    fifoIn      = open("fifo/input", O_RDONLY | O_CLOEXEC);
    if(fifoIn == -1)
    {
        perror("Error: error opening input fifo.");
        return EXIT_FAILURE;
    }
    fifoOut = open("fifo/output", O_WRONLY | O_CLOEXEC);
    if(fifoOut == -1)
    {
        close(fifoIn);
        perror("Error: error opening output fifo.");
        return EXIT_FAILURE;
    }

    if(close(fifoIn) == -1 || close(fifoOut) == -1)
    {
        perror("Error: error closing file descriptor.");
        return EXIT_FAILURE;
    }
    display("Hello, World");
    return EXIT_SUCCESS;
}
