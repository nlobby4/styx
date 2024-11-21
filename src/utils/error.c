#include "utils.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

void exit_error(const char *error_msg, ...)
{
    va_list list;
    va_start(list, error_msg);
    fprintf(stderr, "\033[1;31mERROR:\033[0m ");
    vfprintf(stderr, error_msg, list);
    fputc('\n', stderr);
    va_end(list);
    exit(1);
}

void warning(const char *warning, ...)
{
    va_list list;
    va_start(list, warning);
    printf("\033[1;35mWARNING:\033[0m ");
    vprintf(warning, list);
    putchar('\n');
    va_end(list);
}