/**
 * @file setup.h
 * @author Alexsander d. S. Tenne
 * @brief Contains the functions needed to setup the styx web server.
 * @version 1.0.0
 * @date 2025-03-05
 */
#ifndef _SETUP_H
#define _SETUP_H

#include "buf.h"

/**
 * @brief Creates and returns a pointer to a ipv4 socket address struct.
 * @param ip IPv4 Address in dotted notation.
 * @param port The port to be used.
 * @return A pointer to a singleton socket address struct.
 * Initializes a singleton, zeroes it and sets all values, then returns a
 * pointer to it.
 */
sockaddr_in_p make_ipv4 (server_config config);

message_buffers *setup (int argc, char const **argv);

#endif