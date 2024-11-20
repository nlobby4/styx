#include "utils.h"
#include <stdlib.h>

void handle_args(int argc, char const **argv)
{
    if (argc != 2)
        exit_error("Invalid arguments. Usage: WebServer <port>");
    if ((port = strtol(argv[1], NULL, 10)) == 0)
        exit_error("Invalid port: %u", port);
}