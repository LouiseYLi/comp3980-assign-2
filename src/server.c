#include "../include/convert.h"
#include "../include/display.h"
#include "../include/read.h"
#include "../include/write.h"
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define FIFO_INPUT "./fifo/input"
#define FIFO_OUTPUT "./fifo/output"

typedef char (*convertChar)(char);

static void *handle_client_request(void *arg);
void         handle_signal(int signal);

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
static int terminate = 0;

void handle_signal(int signal)
{
    signal    = 1;
    terminate = signal;
}

static void *handle_client_request(void *arg)
{
    char        currentChar;
    const int  *fds = (int *)arg;
    char        c;
    convertChar convertFunction;
    c = readChar(fds[0]);
    if(c == EOF)
    {
        perror("Error: reading from input FIFO");
        return NULL;
    }
    convertFunction = checkConvertArgs(c);
    if(convertFunction == NULL)
    {
        perror("Error: convert type arguments");
        return NULL;
    }
    if(writeChar(fds[1], convertFunction(c)) == -1)
    {
        perror("Error: error writing to fifo.");
        return NULL;
    }
    while(((currentChar = readChar(fds[0])) != EOF))
    {
        if(writeChar(fds[1], convertFunction(currentChar)) == -1)
        {
            perror("Error: error writing to fifo.");
            return NULL;
        }
        if(currentChar == '\0')
        {
            break;
        }
    }
    return NULL;
}

int main(void)
{
    int       fifoIn;
    int       fifoOut;
    pthread_t thread;
    int       fds[2];

    if(signal(SIGINT, handle_signal) == SIG_ERR)
    {
        perror("Error: setting up signal handler.");
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
        close(fifoIn);
        perror("Error: unable to open input fifo in client.");
        exit(EXIT_FAILURE);
    }

    fds[0] = fifoIn;
    fds[1] = fifoOut;

    while(terminate == 0)
    {
        if(pthread_create(&thread, NULL, handle_client_request, (void *)fds) != 0)
        {
            perror("Error: creating thread");
            continue;
        }
        pthread_join(thread, NULL);
    }

    if(terminate == 1)
    {
        display("Signal received! Terminating...");
    }
    display("server ran successfully");

    close(fifoIn);
    close(fifoOut);
    return EXIT_SUCCESS;
}
