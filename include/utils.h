/**
 * @file utils.h
 * @brief Small utility functions for the WebServer project
 *
 * Contains small helper functions that are used across the project.
 */

#pragma once
#include <signal.h>
#include <inttypes.h>
#include <limits.h>
#include <unistd.h>

/**
 * @brief Port to be used by the server socket
 *
 * Stores a value between 1-65535.
 *
 * @note This value is declared in main.c.
 */
extern uint16_t port;
extern volatile sig_atomic_t running;

/**
 * @brief Prints out a formatted error message to stdout with a newline and exits with error code 1.
 * @param error_msg A pointer to the unformatted message to be printed.
 * @param ... The variable arguments that help format the message.
 */
void exit_error(const char *error_msg, ...);

void handle_args(int argc, char const **argv);

void signal_handler(int signal);