#include "../include/display.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    const char *string;
    const char *conversion;
    int         option;

    const int TOTAL_ARGS = 5;
    if(argc != TOTAL_ARGS)
    {
        perror("Error: invalid number of arguments.");
        return EXIT_FAILURE;
    }

    while((option = getopt(argc, argv, "s:c:")) != -1)
    {
        switch(option)
        {
            case 's':
                string = optarg;
                display(string);
                break;
            case 'c':
                conversion = optarg;
                display(conversion);
                break;
            default:
                perror("Error: invalid options.");
                return EXIT_FAILURE;
        }
    }
    // if (!string || !conversion) {
    //     perror("Error: ")
    // }

    // char c = '0';
    // if(argc > 0)
    // {
    //     return EXIT_SUCCESS;
    // }
    // if(*(argv[0]) == c)
    // {
    //     return EXIT_SUCCESS;
    // }

    // int fifoIn  = -1;
    // int fifoOut = -1;
    // fifoIn      = open("fifo/input", O_RDONLY | O_CLOEXEC);
    // if(fifoIn == -1)
    // {
    //     perror("Error: error opening input fifo.");
    //     return EXIT_FAILURE;
    // }
    // fifoOut = open("fifo/output", O_WRONLY | O_CLOEXEC);
    // if(fifoOut == -1)
    // {
    //     close(fifoIn);
    //     perror("Error: error opening output fifo.");
    //     return EXIT_FAILURE;
    // }

    // if(close(fifoIn) == -1 || close(fifoOut) == -1)
    // {
    //     perror("Error: error closing file descriptor.");
    //     return EXIT_FAILURE;
    // }
    // display("Hello, World");
    display("client ran successfully");
    return EXIT_SUCCESS;
}
