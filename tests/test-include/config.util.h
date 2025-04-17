/**
 * @file config.util.h
 * @author -
 * @brief Contains setup and teardown functions for creating config files
 * unique to the test, aswell as some external variable definitions needed for
 * said tests.
 * @version 1.0.1
 * @date 07/03/2025
 */
#ifndef _CONFIG_UTIL_H
#define _CONFIG_UTIL_H
#include "config.h"

extern char file_name[100];
extern server_config config;
extern const char *argv[];

/**
 * @brief Sets up a config file unique to the process of the running test,
 * using the given port.
 *
 * @param port Port to be used.
 */
extern void setup_config (int port);

/**
 * @brief Runs config_destroy on the server_config instance and removes the
 * file from the directory.
 */
extern void teardown_config (void);

#endif