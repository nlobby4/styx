/**
 * @file response.h
 * @author Alexsander d. S. Tenne
 * @brief Small header file containing the response function, which is to be
 * called after the request function has parsed the client's HTTP-request.
 * @version 1.0.1
 * @date 07/03/2025
 */
#ifndef _RESPONSE_H
#define _RESPONSE_H

#include "buf.h"
#include "header.h"
#include "state.h"

/**
 * @brief This function is to be called only after the request function has
 * been called. It constructs and sends a HTTP-response depending on the header
 * data and the connection state.
 *
 * @param bufs A pointer to the message_buffers instance. Cannot be NULL.
 * @param request_data Parsed HTTP-request data. Can be NULL, if state is
 * anything other than NOT_PROCESSED.
 * @param state Pointer to the connection_state instance. state->code
 * cannot be CLOSE or OK and the pointer cannot be NULL.
 */
void response (message_buffers *bufs, header_data *request_data,
               connection_state *state);
#endif