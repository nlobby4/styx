/**
 * @file utils.h
 * @brief Small utility functions for the WebServer project
 *
 * Contains small helper functions that are used across the project.
 */

#pragma once
#include <signal.h>
#include "types.h"

/**
 * @brief Main loop run condition
 *
 * Set to 1 to run, 0 to stop.
 * @note Important for clean up
 */
extern volatile sig_atomic_t running;
/**
 * @brief Global file descriptor for the listener socket
 */
extern file_descriptor server;
/**
 * @brief Global file descriptor for the connection socket
 */
extern file_descriptor connection;

/**
 * @brief Prints out a formatted error message to stdout with a newline and exits with error code 1.
 * @param error_msg A pointer to the unformatted message to be printed.
 * @param ... The variable arguments that help format the message.
 */
void exit_error(const char *error_msg, ...);

/**
 * @brief Emits a formatted warning message to stdout with a newline.
 * @param warning_msg A pointer to the unformatted message to be printed.
 * @param ... The variable arguments that help format the message.
 */
void warning(const char *warning_msg, ...);

/**
 * @brief Checks if the given arguments are valid. If not, exits with an error message.
 *
 */
const char *handle_args(int argc, char const **argv);

/**
 * @brief Callback function for signal handling.
 * If the given signal is SIGINT, the main loop is exited
 * via the global running variable, thus initializing cleanup
 * and stopping the server.
 * @param signal Input signal
 */
void signal_handler(int signal);
