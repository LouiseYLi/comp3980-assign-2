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

struct client_data
{
    int  fifoIn;
    int  fifoOut;
    char conversion;
};

void handle_signal(int signal)
{
    signal    = 1;
    terminate = signal;
}

static void *handle_client_request(void *arg)
{
    const struct client_data *data = (struct client_data *)arg;
    convertChar               convertFunction;
    char                      currentChar;
    convertFunction = checkConvertArgs(data->conversion);
    if(convertFunction == NULL)
    {
        perror("Error: obtaining specific convert function");
        return NULL;
    }
    while((currentChar = readChar(data->fifoIn)) != EOF)
    {
        if(writeChar(data->fifoOut, convertFunction(currentChar)) == -1)
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
    struct client_data data;
    int                fifoIn;
    int                fifoOut;
    pthread_t          thread;

    int retval = EXIT_SUCCESS;

    if(signal(SIGINT, handle_signal) == SIG_ERR)
    {
        perror("Error: setting up signal handler.");
        retval = EXIT_FAILURE;
        goto done;
    }

    fifoIn = open(FIFO_INPUT, O_RDWR | O_CLOEXEC);
    if(fifoIn == -1)
    {
        perror("Error: unable to open input fifo in client.");
        retval = EXIT_FAILURE;
        goto done;
    }
    fifoOut = open(FIFO_OUTPUT, O_RDWR | O_CLOEXEC);
    if(fifoOut == -1)
    {
        close(fifoIn);
        perror("Error: unable to open input fifo in client.");
        retval = EXIT_FAILURE;
        goto done;
    }

    data.fifoIn  = fifoIn;
    data.fifoOut = fifoOut;

    while(terminate == 0)
    {
        char conversion;
        conversion = readChar(data.fifoIn);
        if(conversion != EOF)
        {
            data.conversion = conversion;
            if(pthread_create(&thread, NULL, handle_client_request, (void *)&data) != 0)
            {
                perror("Error: creating thread");
                retval = EXIT_FAILURE;
                goto cleanup;
            }
            if(pthread_join(thread, NULL) != 0)
            {
                perror("Error: pthread_join in server.");
                retval = EXIT_FAILURE;
                goto cleanup;
            }
        }
    }

    if(terminate == 1)
    {
        display("Signal received! Terminating...");
    }
    display("server ran successfully");
cleanup:
    close(fifoIn);
    close(fifoOut);
done:
    return retval;
}
