#include "utils.h"

void signal_handler(int signal)
{
    running = 0;
}