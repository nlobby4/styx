#pragma once
#include <sys/types.h>
#include <inttypes.h>

typedef struct
{
    uint16_t port;
    char addr[16];
    size_t recv_header_sz;
    size_t recv_body_sz;
    size_t resp_header_sz;
    size_t resp_body_sz;
    time_t timeout_s;
    int max_clients;
} server_config;

server_config *load_config(void);
void free_config(server_config *config);