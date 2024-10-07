#include "../include/read.h"

// char *readStr(int fifo)
// {
//     const size_t INITIAL_BUF_SIZE = 128;
//     size_t       total_bytes_read = 0;
//     ssize_t      bytes_read       = 0;
//     char        *buf              = (char *)malloc(INITIAL_BUF_SIZE);
//     display("after first malloc...");
//     if(buf == NULL)
//     {
//         perror("Error: malloc buffer in readStr()");
//         free(buf);
//         errno = 3;
//         return 0;
//     }
//     do
//     {
//         display("in beginning of loop...");
//         bytes_read = read(fifo, buf, INITIAL_BUF_SIZE + total_bytes_read);
//         if(bytes_read == -1)
//         {
//             perror("Error: reading from fifo.");
//             errno = 2;
//             free(buf);
//             return 0;
//         }
//         total_bytes_read += (size_t)bytes_read;
//         if((size_t)bytes_read >= INITIAL_BUF_SIZE)
//         {
//             char *tempBuf = (char *)realloc(buf, total_bytes_read);
//             display("in if...");
//             if(tempBuf == NULL)
//             {
//                 perror("Error: malloc buffer in readStr()");
//                 errno = 3;
//                 free(tempBuf);
//                 free(buf);
//                 return 0;
//             }
//             buf = tempBuf;
//         }
//         display(buf);
//         display("at end of loop...");
//         displaySize(bytes_read);
//     } while(bytes_read > 0);

//     return buf;
// }

char readChar(int fifo)
{
    char    c;
    ssize_t bytes_read = read(fifo, &c, sizeof(char));
    if(bytes_read == -1)
    {
        perror("Error: reading file.");
        return EOF;
    }
    if(bytes_read == 0)
    {
        return EOF;
    }
    if(c == '\0')
    {
        return c;
    }
    return c;
}
