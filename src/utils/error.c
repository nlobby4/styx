#include "utils.h"
#include "types.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>

void exit_error(const char *error_msg, ...)
{
    va_list list;
    va_start(list, error_msg);
    fprintf(stderr, "\033[1;31mERROR:\033[0m ");
    if (errno)
        perror(NULL);
    vfprintf(stderr, error_msg, list);
    fputc('\n', stderr);
    va_end(list);
    if (server != -1 && server != 0)
        close(server);
    if (connection != -1 && connection != 0)
        close(connection);
    exit(1);
}

void warning(const char *warning, ...)
{
    va_list list;
    va_start(list, warning);
    if (errno)
        errno = 0;
    printf("\033[1;35mWARNING:\033[0m ");
    vprintf(warning, list);
    putchar('\n');
    va_end(list);
}