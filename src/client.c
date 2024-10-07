#include "../include/convert.h"
#include "../include/display.h"
#include "../include/read.h"
#include "../include/write.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define FIFO_INPUT "./fifo/input"
#define FIFO_OUTPUT "./fifo/output"

// #define FIFO_OUTPUT "./fifo/output"

int main(int argc, char *argv[])
{
    int  option;
    int  fifoIn;
    int  fifoOut;
    char currentChar;

    const int   TOTAL_ARGS    = 5;
    const char *string        = NULL;
    char        conversion    = ' ';
    int         blockExecuted = 0;
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
                break;
            case 'c':
                conversion = getConvertOption(optarg);
                break;
            default:
                perror("Error: invalid options.");
                exit(EXIT_FAILURE);
        }
    }
    if(string == NULL || conversion == ' ')
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
    fifoOut = open(FIFO_OUTPUT, O_RDWR | O_CLOEXEC);
    if(fifoOut == -1)
    {
        perror("Error: unable to open output fifo in client.");
        close(fifoIn);
        exit(EXIT_FAILURE);
    }

    writeChar(fifoIn, conversion);
    writeStr(fifoIn, string);
    writeChar(fifoIn, '\0');

    while((currentChar = readChar(fifoOut)) != EOF)
    {
        if(blockExecuted == 0)
        {
            blockExecuted = 1;
            continue;
        }
        if(currentChar == '\0')
        {
            break;
        }
        displayChar(currentChar);
    }

    display("client ran successfully");
    close(fifoIn);
    close(fifoOut);

    return EXIT_SUCCESS;
}
