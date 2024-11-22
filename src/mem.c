#include "types.h"
#include "mem.h"
#include "config.h"
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

sockaddr_in_p make_ipv4(server_config *config)
{
    static struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(config->port);
    int err = inet_pton(AF_INET, config->addr, &addr.sin_addr.s_addr);
    if (err == 0 || err == -1)
    {
        free(config);
        exit_error("invalid ip address.");
    }
    return &addr;
}

buffers *make_buffers(server_config *config)
{
    static buffers bufs;
    bufs.recv_head_sz = config->recv_header_sz;
    bufs.recv_body_sz = config->recv_body_sz;
    bufs.resp_head_sz = config->resp_header_sz;
    bufs.resp_body_sz = config->resp_body_sz;
    bufs.recv_head = calloc(config->recv_header_sz, sizeof(char));
    bufs.recv_body = calloc(config->recv_body_sz, sizeof(char));
    bufs.resp_head = calloc(config->resp_header_sz, sizeof(char));
    bufs.resp_body = calloc(config->resp_body_sz, sizeof(char));
    if (!bufs.recv_head || !bufs.recv_body || !bufs.resp_head || !bufs.resp_body)
    {
        free_bufs(&bufs);
        free(config);
        exit_error("buffer cannot be allocated. Check serverconfig.json.");
    }
    return &bufs;
}

void free_bufs(buffers *bufs)
{
    if (bufs->recv_head)
    {
        free(bufs->recv_head);
        bufs->recv_head = NULL;
    }
    if (bufs->recv_body)
    {
        free(bufs->recv_body);
        bufs->recv_body = NULL;
    }
    if (bufs->resp_head)
    {
        free(bufs->resp_head);
        bufs->resp_head = NULL;
    }
    if (bufs->resp_body)
    {
        free(bufs->resp_body);
        bufs->resp_body = NULL;
    }
}