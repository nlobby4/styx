/**
 * @file globals.h
 * @author Alexsander d. S. Tenne
 * @brief Contains all global variables, which are all initialized in the setup
 * function.
 * @version 1.0.1
 * @date 07/03/2025
 */
#ifndef _GLOBALS_H
#define _GLOBALS_H

#include "global_types.h"
#include <arpa/inet.h>
#include <signal.h>

/**
 * @brief Main loop run condition.
 *
 * @note Important for clean up. The signal handlers for SIGINT and SIGTERM set
 * this to 0.
 */
extern volatile sig_atomic_t running;

/// @brief File descriptor for the listener socket.
extern file_descriptor server;

/// @brief File descriptor for the connection socket.
extern file_descriptor connection;

/// @brief Interval for server time outs.
extern struct timeval interval;

/// @brief Client address struct.
extern struct sockaddr_in client_addr;

/// @brief Client socket address length.
extern socklen_t addr_len;

/// @brief Process id.
extern pid_t pid;

#endif