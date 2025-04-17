/**
 * @file cleanup.h
 * @author -
 * @brief Small header file containing the cleanup function, which is to be
 * called before exiting the program, in order to free up any ressources.
 * @version 1.0.1
 * @date 07/03/2025
 */
#ifndef _CLEANUP_H
#define _CLEANUP_H

#include "buf.h"

/**
 * @brief Clean up function for when the process finishes. Closes up open file
 * descriptors and, if inside a child process, frees up the memory inside bufs.
 *
 * @param bufs The message_buffers instance to free the dynamically allocated
 * memory of. Cannot be NULL.
 */
void cleanup (message_buffers *bufs);

#endif