#include "utils.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

void exit_error(const char *error_msg, ...)
{
    va_list list;
    va_start(list, error_msg);
    vfprintf(stderr, error_msg, list);
    fputc('\n', stderr);
    va_end(list);
    exit(1);
}