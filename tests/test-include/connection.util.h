/**
 * @file config.util.h
 * @author Alexsander d. S. Tenne
 * @brief Contains setup and teardown functions for setting and cleaning up a
 * connection to the server via a seperate thread.
 * @version 1.0.1
 * @date 07/03/2025
 */
#ifndef _CONNECTION_UTIL_H
#define _CONNECTION_UTIL_H
#include "buf.h"
#include <arpa/inet.h>

extern file_descriptor test_client;
extern struct sockaddr_in server_addr;
extern message_buffers *bufs;

/**
 * @brief Creates a unique config file based using the input port and makes a
 * thread connect to the server and send the given input message.
 *
 * @param port Port to be used.
 * @param msg Message string to be sent.
 * @param connect_to_server Function to be run by the thread.
 */
extern void setup_connection (int port, char *msg,
                              void *(*connect_to_server) (void *));

/**
 * @brief Cleans up the thread, closes the connection on the client side and
 * removes the config file from the directory.
 */
extern void teardown_connection (void);

#endif