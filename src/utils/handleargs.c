#include "utils.h"
#include <stdlib.h>

const char *handle_args(int argc, char const **argv)
{
    if (argc > 2)
        exit_error("Invalid arguments. Usage: WebServer <opt: config_path>");
    if (argc == 2)
        return argv[1];
    return NULL;
}