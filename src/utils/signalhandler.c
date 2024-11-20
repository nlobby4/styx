#include "utils.h"
#include <stdio.h>

void signal_handler(int signal)
{
    // printf("\nShutting down...\n");
    running = 0;
}