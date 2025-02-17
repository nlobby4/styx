#define _GNU_SOURCE
#include "types.h"
#include "mem.h"
#include "config.h"
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/mman.h>

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
        exit_error("Invalid ip address.");
    }
    return &addr;
}

static int allocate_buffer(buffer *buf, long size)
{
    buf->size = size;
    buf->payload = mmap(NULL, sizeof(char), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    return buf->payload != MAP_FAILED;
}

message_buffers *make_buffers(server_config *config)
{
    static message_buffers bufs;
    memset(&bufs, 0, sizeof(bufs));
    int recv_head = allocate_buffer(&bufs.recv.head, config->recv_header_sz);
    int recv_body = allocate_buffer(&bufs.recv.body, config->recv_body_sz);
    int resp_head = allocate_buffer(&bufs.resp.head, config->resp_header_sz);
    int resp_body = allocate_buffer(&bufs.resp.body, config->resp_body_sz);

    if (!recv_head || !recv_body || !resp_head || !resp_body)
    {
        free_bufs(&bufs);
        free(config);
        exit_error("HTTP message buffers cannot be allocated. Check serverconfig.json.");
    }
    return &bufs;
}

static void deallocate_buffer(buffer *buf)
{
    if (buf->payload && munmap(buf->payload, buf->size) < 0)
        warning("Buffer of size %ld cannot be freed.", buf->size);
    buf->payload = NULL;
}

void free_bufs(message_buffers *bufs)
{
    deallocate_buffer(&bufs->resp.head);
    deallocate_buffer(&bufs->resp.body);
    deallocate_buffer(&bufs->recv.head);
    deallocate_buffer(&bufs->recv.body);
}