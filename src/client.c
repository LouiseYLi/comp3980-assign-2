#include "../include/display.h"
#include "../include/read.h"
#include "../include/write.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define FIFO_INPUT "./fifo/input"

int main(int argc, char *argv[])
{
    int option;
    int fifoIn;
    // int fifoOut;

    const int   TOTAL_ARGS = 5;
    const char *string     = NULL;
    const char *conversion = NULL;
    if(argc != TOTAL_ARGS)
    {
        perror("Error: invalid number of arguments.");
        exit(EXIT_FAILURE);
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
                exit(EXIT_FAILURE);
        }
    }
    if(string == NULL || conversion == NULL)
    {
        perror("Error: error assigning arguments.");
        exit(EXIT_FAILURE);
    }

    fifoIn = open(FIFO_INPUT, O_RDWR | O_CLOEXEC);
    if(fifoIn == -1)
    {
        perror("Error: unable to open input fifo in client.");
        exit(EXIT_FAILURE);
    }

    writeStr(fifoIn, string);
    readStr(fifoIn, sizeof(string));
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
    close(fifoIn);

    return EXIT_SUCCESS;
}
