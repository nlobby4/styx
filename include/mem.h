#pragma once
#include "types.h"
#include "config.h"
typedef struct
{
    char *recv_head;
    char *recv_body;
    char *resp_head;
    char *resp_body;
} buffers;
/**
 * @brief Creates and returns a pointer to a ipv4 socket address struct.
 * @param ip IPv4 Address in dotted notation.
 * @param port The port to be used.
 * @return A pointer to a singleton socket address struct.
 * Initializes a singleton, zeroes it and sets all values, then returns a pointer to it.
 */
sockaddr_in_p make_ipv4(server_config *config);

buffers *make_buffers(server_config *config);
void free_bufs(buffers *bufs);