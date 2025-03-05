/**
 * @file run.h
 * @author Alexsander d. S. Tenne
 * @brief Small header file containing the run function, which runs the main
 * loop of the program.
 * @version 1.0.0
 * @date 2025-03-05
 */
#ifndef _RUN_H
#define _RUN_H
#include "buf.h"

/**
 * @brief Runs the main loop. Only call this function after the setup function.
 * @see setup
 * @param bufs The initialized message_buffers instance. Cannot be NULL.
 */
void run (message_buffers *bufs);

#endif