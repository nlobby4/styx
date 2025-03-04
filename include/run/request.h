#ifndef _REQUEST_H
#define _REQUEST_H
#include "header.h"
#include "mem.h"
#include "state.h"

/**
 * @brief Reads from the connection socket and processes the http request.
 *
 * @param bufs A pointer to the initialized message_buffers struct, where the
 * request header and body strings will be stored.
 * @param state A pointer to the initialized connection_state struct, which
 * decides the kind of response that will be sent back.
 * @return A pointer to the processed header_data object.
 */
header_data *request (message_buffers *bufs, connection_state *state);

#endif