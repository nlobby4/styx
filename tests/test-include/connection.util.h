#ifndef _CONNECTION_UTIL_H
#define _CONNECTION_UTIL_H
#include "mem.h"
#include <arpa/inet.h>

extern file_descriptor test_client;
extern struct sockaddr_in server_addr;
extern message_buffers *bufs;

extern void setup_connection (int port, char *msg,
                              void *(*connect_to_server) (void *));
extern void teardown_connection (void);

#endif