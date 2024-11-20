#include "utils.h"
#include <stdio.h>

void signal_handler(int signal)
{
    printf("Received signal %d; shutting down...\n", signal);
    running = 0;
}