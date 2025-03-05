/**
 * @file config.util.h
 * @author Alexsander d. S. Tenne
 * @brief Contains setup and teardown functions for creating config files
 * unique to the test, aswell as some external variable definitions needed for
 * said tests.
 * @version 1.0.0
 * @date 2025-03-05
 */
#ifndef _CONFIG_UTIL_H
#define _CONFIG_UTIL_H
#include "config.h"

extern char file_name[100];
extern server_config config;
extern const char *argv[];

extern void setup_config (int port);
extern void teardown_config (void);

#endif