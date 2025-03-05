/**
 * @file config.util.h
 * @author Alexsander d. S. Tenne
 * @brief Contains setup and teardown functions for setting and cleaning up a
 * connection to the server via a seperate thread.
 * @version 1.0.0
 * @date 2025-03-05
 */
#ifndef _CONNECTION_UTIL_H
#define _CONNECTION_UTIL_H
#include "buf.h"
#include <arpa/inet.h>

extern file_descriptor test_client;
extern struct sockaddr_in server_addr;
extern message_buffers *bufs;

extern void setup_connection (int port, char *msg,
                              void *(*connect_to_server) (void *));
extern void teardown_connection (void);

#endif