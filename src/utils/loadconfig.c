#include "config.h"
#include "utils.h"
#include <stdlib.h>
#include <cjson/cJSON.h>

server_config *load_config()
{
    server_config *config = calloc(1, sizeof(*config));
    if (!config)
    {
        exit_error("Malloc failed in load_config()");
    }
}