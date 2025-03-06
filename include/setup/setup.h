/**
 * @file setup.h
 * @author Alexsander d. S. Tenne
 * @brief Contains the functions needed to setup the styx web server.
 * @version 1.0.1
 * @date 07/03/2025
 */
#ifndef _SETUP_H
#define _SETUP_H

#include "buf.h"

/**
 * @brief Creates and returns a pointer to a socket address instance.
 *
 * @param config A pointer to the server_config instance. Cannot be NULL.
 * @return A pointer to a singleton socket address struct.
 */
sockaddr_in_p make_ipv4 (server_config config);

/**
 * @brief Sets up the server based on the configuration
 * file, whose path is optionally specified in argv. All global values are
 * initialized aswell.
 *
 * @param argc Argument counter.
 * @param argv Argument vector.
 *
 * @return message_buffers * A pointer to the setup message_buffers instance.
 * No dynamic memory has been allocated inside of it yet.
 */
message_buffers *setup (int argc, char const **argv);

#endif