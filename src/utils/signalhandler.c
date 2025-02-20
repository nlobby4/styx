#include "utils.h"

void
signal_handler (int signal)
{
  if (signal == SIGINT)
    running = 0;
}