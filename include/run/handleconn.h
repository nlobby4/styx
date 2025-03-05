/**
 * @file handleconn.h
 * @author Alexsander d. S. Tenne
 * @brief Small header file containing the handle_connection function, which is
 * to be called when the listener socket has accepted a connection.
 * @version 1.0.0
 * @date 2025-03-05
 */
#ifndef _HANDLECONN_H
#define _HANDLECONN_H
#include "buf.h"
#include "global_types.h"

/**
 * @brief When a connection is established and the process is forked, this
 * function is to be run with a previously setup message buffer struct.
 *
 * @param bufs A pointer to the initialized message_buffers struct, where the
 * HTTP-request and -response will be stored.
 */
void handle_connection (message_buffers *bufs);

#endif